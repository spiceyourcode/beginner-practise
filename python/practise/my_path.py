from pathlib import Path
import os 
new_path = Path('C:/Users/Administrator/Desktop/Mayai')
new_file = Path('C:/Users/Administrator/Desktop/Mayai/mayai.py')


try :                
    if new_path.exists():
        os.remove(new_path)
        print('folder deleted successfully')
    else:
        print(f'the folder{new_path} does not exist and can potentially be removed')
    
except Exception as e:
    print(f'An error occurred: {e}')
else:
    print('Folder deleted successfully')


 

