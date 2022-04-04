
import sys
import re

# files = [ "247-0.txt", "248-0.txt" ]

cleanFile = open("noah_webster_1913_dictionary_one_word_per_line_unique_cleaned_up_2.txt", "w")

allWords = []
previousWords = {}

with open("noah_webster_1913_dictionary_one_word_per_line_unique_cleaned_up.txt", "r") as file:
    words = file.readlines()
    for word in words:
        goodword = re.split(r"^[^a-zA-Z]*", re.split(r"[^a-zA-Z]*$", word)[0])[-1]
        if len(goodword) and (not goodword.lower() in previousWords):
            previousWords[goodword.lower()] = True
            allWords.append(goodword)

allWords.sort()

for word in allWords:
    cleanFile.write(word + "\n")

sys.exit()

goodFile = open("noah_webster_1913_dictionary.txt", "w")

with open("pg29765.txt") as fileHandle:
    badlines = fileHandle.readlines()
    for badline in badlines:
        goodline = ""
        for letter in badline:
            if (len(re.findall("[a-zA-Z \-'_0-9,.;:!?()]", letter))):
                goodline += letter
        goodFile.write(goodline + "\n");

wordsFile = open("noah_webster_1913_dictionary_one_word_per_line.txt", "w")

with open("noah_webster_1913_dictionary.txt") as fileHandle:
    lines = fileHandle.readlines()
    for line in lines:
        word = ""
        for letter in line:
            if (len(re.findall("[a-zA-Z\-'_0-9,.;:!?()]", letter))):
                word += letter
            else:
                wordsFile.write(word + "\n");
                word = ""

uniqueWordsFile = open("noah_webster_1913_dictionary_one_word_per_line_unique.txt", "w")

unique_words = {}

with open("noah_webster_1913_dictionary_one_word_per_line.txt") as fileHandle:
    lines = fileHandle.readlines()
    for line in lines:
        word = ""
        for letter in line:
            if (len(re.findall("[a-zA-Z\-'_0-9,.;:!?()]", letter))):
                word += letter
            else:
                unique_words[word] = True
                word = ""

for word, v in unique_words.items():
    uniqueWordsFile.write(word + "\n")
