def scroll(displayLength, message):
    messageList = []

    if len(message) <= displayLength:
        messageList.append(message)
        return messageList

    for x in range((len(message) - displayLength)+1):
        messageList.append(message[x:x+displayLength])

    return messageList



