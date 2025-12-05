#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BLOCK_SIZE 256
#define MAX_FILENAME_LENGTH 56
#define MAX_USERNAME_LENGTH 40
#define POINTERS_PER_BLOCK 8

typedef struct {
    char fileName[MAX_FILENAME_LENGTH];
    int inodeIndex;
} FileNameEntry;

typedef struct {
    int fileSize;
    time_t lastModified;
    int blockPointers[POINTERS_PER_BLOCK - 1];
    int nextPointer;
    char userName[MAX_USERNAME_LENGTH];
} InodeEntry;

typedef struct {
    int numBlocks;
    FileNameEntry *fnt;
    InodeEntry *dabpt;
    char *dataBlocks;
} FileSystem;

FileSystem *createFileSystem(int numBlocks) {
    FileSystem *fs = (FileSystem *)malloc(sizeof(FileSystem));
    fs->numBlocks = numBlocks;
    fs->fnt = (FileNameEntry *)malloc(sizeof(FileNameEntry) * numBlocks);
    fs->dabpt = (InodeEntry *)malloc(sizeof(InodeEntry) * numBlocks);
    fs->dataBlocks = (char *)malloc(BLOCK_SIZE * numBlocks);

    memset(fs->fnt, 0, sizeof(FileNameEntry) * numBlocks);
    memset(fs->dabpt, 0, sizeof(InodeEntry) * numBlocks);
    memset(fs->dataBlocks, 0, BLOCK_SIZE * numBlocks);

    return fs;
}

void formatFileSystem(FileSystem *fs, int numFiles, int numDabptEntries) {
    int i;
	for (i = 0; i < numFiles; i++) {
        strcpy(fs->fnt[i].fileName, "");
        fs->fnt[i].inodeIndex = -1;
    }

    for (i = 0; i < numDabptEntries; i++) {
        fs->dabpt[i].fileSize = 0;
        fs->dabpt[i].lastModified = 0;
        memset(fs->dabpt[i].blockPointers, -1, sizeof(fs->dabpt[i].blockPointers));
        fs->dabpt[i].nextPointer = -1;
        strcpy(fs->dabpt[i].userName, "");
    }
}

void saveFileSystem(FileSystem *fs, const char *fileName) {
    FILE *file = fopen(fileName, "wb");
    if (file) {
        fwrite(fs, sizeof(FileSystem), 1, file);
        fwrite(fs->fnt, sizeof(FileNameEntry), fs->numBlocks, file);
        fwrite(fs->dabpt, sizeof(InodeEntry), fs->numBlocks, file);
        fwrite(fs->dataBlocks, BLOCK_SIZE, fs->numBlocks, file);
        fclose(file);
    }
}

FileSystem *openFileSystem(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (!file) {
        return NULL;
    }

    FileSystem *fs = (FileSystem *)malloc(sizeof(FileSystem));
    fread(fs, sizeof(FileSystem), 1, file);

    fs->fnt = (FileNameEntry *)malloc(sizeof(FileNameEntry) * fs->numBlocks);
    fs->dabpt = (InodeEntry *)malloc(sizeof(InodeEntry) * fs->numBlocks);
    fs->dataBlocks = (char *)malloc(BLOCK_SIZE * fs->numBlocks);

    fread(fs->fnt, sizeof(FileNameEntry), fs->numBlocks, file);
    fread(fs->dabpt, sizeof(InodeEntry), fs->numBlocks, file);
    fread(fs->dataBlocks, BLOCK_SIZE, fs->numBlocks, file);

    fclose(file);
    return fs;
}

void listFiles(FileSystem *fs) {
	int i;
    for (i = 0; i < fs->numBlocks; i++) {
        if (fs->fnt[i].inodeIndex != -1) {
            printf("File: %s, Inode: %d\n", fs->fnt[i].fileName, fs->fnt[i].inodeIndex);
        }
    }
}

void removeFile(FileSystem *fs, const char *fileName) {
	int i;
    for (i = 0; i < fs->numBlocks; i++) {
        if (strcmp(fs->fnt[i].fileName, fileName) == 0) {
            int inodeIndex = fs->fnt[i].inodeIndex;
            if (inodeIndex != -1) {
                memset(&fs->dabpt[inodeIndex], 0, sizeof(InodeEntry));
                memset(&fs->fnt[i], 0, sizeof(FileNameEntry));
                printf("File %s removed.\n", fileName);
            }
            return;
        }
    }
    printf("File %s not found.\n", fileName);
}

void renameFile(FileSystem *fs, const char *oldName, const char *newName) {
    int i;
	for (i = 0; i < fs->numBlocks; i++) {
        if (strcmp(fs->fnt[i].fileName, oldName) == 0) {
            strncpy(fs->fnt[i].fileName, newName, MAX_FILENAME_LENGTH);
            printf("File %s renamed to %s.\n", oldName, newName);
            return;
        }
    }
    printf("File %s not found.\n", oldName);
}

void putFile(FileSystem *fs, const char *externalFileName) {
    FILE *externalFile = fopen(externalFileName, "rb");
    if (!externalFile) {
        printf("Failed to open external file %s.\n", externalFileName);
        return;
    }

    fseek(externalFile, 0, SEEK_END);
    int fileSize = ftell(externalFile);
    fseek(externalFile, 0, SEEK_SET);

    char *fileBuffer = (char *)malloc(fileSize);
    fread(fileBuffer, 1, fileSize, externalFile);
    fclose(externalFile);

    // Find an empty slot in the FNT and DABPT
    int fntIndex = -1;
    int inodeIndex = -1;
    int i;
    for (i = 0; i < fs->numBlocks; i++) {
        if (fs->fnt[i].inodeIndex == -1) {
            fntIndex = i;
            break;
        }
    }
    for (i = 0; i < fs->numBlocks; i++) {
        if (fs->dabpt[i].fileSize == 0) {
            inodeIndex = i;
            break;
        }
    }

    if (fntIndex == -1 || inodeIndex == -1) {
        printf("No space available in the file system.\n");
        free(fileBuffer);
        return;
    }

    strncpy(fs->fnt[fntIndex].fileName, externalFileName, MAX_FILENAME_LENGTH);
    fs->fnt[fntIndex].inodeIndex = inodeIndex;

    InodeEntry *inode = &fs->dabpt[inodeIndex];
    inode->fileSize = fileSize;
    inode->lastModified = time(NULL);
    // Assign blocks and copy data
    int blocksRequired = (fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int blockIndex = 0;
    for (i = 0; i < blocksRequired; i++) {
        if (i < POINTERS_PER_BLOCK - 1) {
            inode->blockPointers[i] = blockIndex++;
        } else {
            // Handle chaining for large files
            inode->nextPointer = blockIndex++;
        }
        memcpy(fs->dataBlocks + inode->blockPointers[i] * BLOCK_SIZE, fileBuffer + i * BLOCK_SIZE, BLOCK_SIZE);
    }
    free(fileBuffer);
    printf("File %s stored in the file system.\n", externalFileName);
}

void getFile(FileSystem *fs, const char *fileName) {
	int i;
    for (i = 0; i < fs->numBlocks; i++) {
        if (strcmp(fs->fnt[i].fileName, fileName) == 0) {
            int inodeIndex = fs->fnt[i].inodeIndex;
            if (inodeIndex != -1) {
                InodeEntry *inode = &fs->dabpt[inodeIndex];
                char *fileBuffer = (char *)malloc(inode->fileSize);
                int blocksRequired = (inode->fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
                int j;
				for (j = 0; j < blocksRequired; j++) {
                    if (j < POINTERS_PER_BLOCK - 1) {
                        memcpy(fileBuffer + j * BLOCK_SIZE, fs->dataBlocks + inode->blockPointers[j] * BLOCK_SIZE, BLOCK_SIZE);
                    } else {
                        // Handle chaining for large files
                        memcpy(fileBuffer + j * BLOCK_SIZE, fs->dataBlocks + inode->nextPointer * BLOCK_SIZE, BLOCK_SIZE);
                    }
                }
                FILE *externalFile = fopen(fileName, "wb");
                fwrite(fileBuffer, 1, inode->fileSize, externalFile);
                fclose(externalFile);
                free(fileBuffer);
                printf("File %s retrieved from the file system.\n", fileName);
            }
            return;
        }
    }
    printf("File %s not found.\n", fileName);
}

void setUser(FileSystem *fs, const char *fileName, const char *userName) {
    int i;
	for (i = 0; i < fs->numBlocks; i++) {
        if (strcmp(fs->fnt[i].fileName, fileName) == 0) {
            int inodeIndex =fs->fnt[i].inodeIndex;
            
            

if (inodeIndex != -1) {
strncpy(fs->dabpt[inodeIndex].userName, userName, MAX_USERNAME_LENGTH);
printf("User %s set for file %s.\n", userName, fileName);
}
return;
}
}
printf("File %s not found.\n", fileName);
}

void linkFile(FileSystem *fs, const char *fileName, const char *linkName) {
int fntIndex = -1;
int i;
for (i = 0; i < fs->numBlocks; i++) {
if (fs->fnt[i].inodeIndex == -1) {
fntIndex = i;
break;
}
}

if (fntIndex == -1) {
    printf("No space available for linking the file.\n");
    return;
}

for (i = 0; i < fs->numBlocks; i++) {
    if (strcmp(fs->fnt[i].fileName, fileName) == 0) {
        strncpy(fs->fnt[fntIndex].fileName, linkName, MAX_FILENAME_LENGTH);
        fs->fnt[fntIndex].inodeIndex = fs->fnt[i].inodeIndex;
        printf("File %s linked as %s.\n", fileName, linkName);
        return;
    }
}
printf("File %s not found.\n", fileName);
}

void unlinkFile(FileSystem *fs, const char *linkName) {
	int i;
	for ( i = 0; i < fs->numBlocks; i++) {
if (strcmp(fs->fnt[i].fileName, linkName) == 0) {
fs->fnt[i].inodeIndex = -1;
memset(&fs->fnt[i], 0, sizeof(FileNameEntry));
printf("Link %s removed.\n", linkName);
return;
}
}
printf("Link %s not found.\n", linkName);
}

int main(int argc, char *argv[]) {
if (argc < 2) {
printf("Usage: %s  [arguments]\n", argv[0]);
return 1;
}

static FileSystem *fs = NULL;

if (strcmp(argv[1], "createfs") == 0) {
    if (argc != 3) {
        printf("Usage: %s createfs <numBlocks>\n", argv[0]);
        return 1;
    }
    int numBlocks = atoi(argv[2]);
    fs = createFileSystem(numBlocks);
    formatFileSystem(fs, numBlocks, numBlocks);
    printf("File system created with %d blocks.\n", numBlocks);
} else if (strcmp(argv[1], "savefs") == 0) {
    if (argc != 3) {
        printf("Usage: %s savefs <fileName>\n", argv[0]);
        return 1;
    }
    const char *fileName = argv[2];
    saveFileSystem(fs, fileName);
    printf("File system saved to %s.\n", fileName);
} else if (strcmp(argv[1], "openfs") == 0) {
    if (argc != 3) {
        printf("Usage: %s openfs <fileName>\n", argv[0]);
        return 1;
    }
    const char *fileName = argv[2];
    fs = openFileSystem(fileName);
    if (fs == NULL) {
        printf("Failed to open file system from %s.\n", fileName);
        return 1;
    }
    printf("File system opened from %s.\n", fileName);
} else if (strcmp(argv[1], "ls") == 0) {
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    listFiles(fs);
} else if (strcmp(argv[1], "rm") == 0) {
    if (argc != 3) {
        printf("Usage: %s rm <fileName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *fileName = argv[2];
    removeFile(fs, fileName);
} else if (strcmp(argv[1], "mv") == 0) {
    if (argc != 4) {
        printf("Usage: %s mv <oldName> <newName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *oldName = argv[2];
    const char *newName = argv[3];
    renameFile(fs, oldName, newName);
} else if (strcmp(argv[1], "put") == 0) {
    if (argc != 3) {
        printf("Usage: %s put <externalFileName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *externalFileName = argv[2];
    putFile(fs, externalFileName);
} else if (strcmp(argv[1], "get") == 0) {
    if (argc != 3) {
        printf("Usage: %s get <fileName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *fileName = argv[2];
    getFile(fs, fileName);
} else if (strcmp(argv[1], "setuser") == 0) {
    if (argc != 4) {
        printf("Usage: %s setuser <fileName> <userName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *fileName = argv[2];
    const char *userName = argv[3];
    setUser(fs, fileName, userName);
} else if (strcmp(argv[1], "link") == 0) {
    if (argc != 4) {
        printf("Usage: %s link <fileName> <linkName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *fileName = argv[2];
    const char *linkName = argv[3];
    linkFile(fs, fileName, linkName);
} else if (strcmp(argv[1], "unlink") == 0) {
    if (argc != 3) {
        printf("Usage: %s unlink <linkName>\n", argv[0]);
        return 1;
    }
    if (fs == NULL) {
        printf("No file system is loaded.\n");
        return 1;
    }
    const char *linkName = argv[2];
    unlinkFile(fs, linkName);
} else {
    printf("Unknown command: %s\n", argv[1]);
    return 1;
}

return 0;
}            