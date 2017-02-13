##example API call
##https://translation.googleapis.com/language/translate/v2?key=                    &source=en&target=de&q=Hello%20world&q=My%20name%20is%20Jeff

from urllib import request
import json, html
from gtts import gTTS

##this is my google translate key
myKey = "PUT YOUR KEY HERE"

##phrases is a list of phrases to translate
def text(phrases, source, target):
    ##the url starts as below and then a query is appended for each word. Only a since call is made to google
    myUrl = "https://translation.googleapis.com/language/translate/v2?key={}&source={}&target={}".format(myKey, source, target)

    ##results will be a list of dictionaries
    results = []

    ##query each phrase in list
    for phrase in phrases:
        myUrl += "&q={}".format(phrase.replace(" ", "%20"))

    ##make request to google and convert from JSON to python Dict
    jsonObj = request.urlopen(myUrl)
    myData = json.load(jsonObj)

    ##note: html.unescape can be used make look nice if printing to console
    ##go through each phrase in phrases and use that as the key to the corresponding translation
    for i in range(len(myData["data"]["translations"])):
        results.append({
            "source": phrases[i],
            "target": myData["data"]["translations"][i]["translatedText"]
        })

    ##return our results
    return results

def audio(phrases, source, target, path):
    ##first get translation for text function
    results = text(phrases, source, target)

    ##go through each val in results
    for result in results:
        ##unecape html to make titles look nice
        resultsText = html.unescape(result["target"])
        ##print out to console
        print("text: ", resultsText)
        ####now use gTTS
        tts = gTTS(text=resultsText, lang=target)
        ##save in path directory
        tts.save("{}.mp3".format(path + resultsText))