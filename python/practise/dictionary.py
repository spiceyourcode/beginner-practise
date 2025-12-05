def emoji_converter(message):
   
    msg2=message.split(' ')
    
    emojis_mapping={
        "sad":"😒",
        "happy":"😂",
        "laughing":"🤣🤣",
        "angry":"😡😡"
        
    }
    output=""
    for words in msg2:
        output += emojis_mapping.get(words,words) + " "
    return output    
message=input("enter your message:")

print(emoji_converter(message))


