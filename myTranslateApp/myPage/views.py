from django.shortcuts import render
from django.http import JsonResponse
from . import googletranslate


def myPage(request):
    ##talk to server
    print("hello server!")
    ##get query
    (source, target, text) = (request.GET.get('source', ''), request.GET.get('target', ''), request.GET.get('text', '') )
    ##if there is no query then just render the form
    if not source or not target or not text:
        print("incomplete request")
        return render(request, "myPage/myPage.html")

    ##translate from english to thai
    myText = googletranslate.text([text], source, target)
    ##download file to server
    googletranslate.audio([text], source, target, "myPage/static/mp3s/" )
    ##print to server
    print("myText", myText)
    ##return json object to javascript
    return JsonResponse({ "translations": myText})