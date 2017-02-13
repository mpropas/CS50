
//let the DOM load
$(document).ready(function () {

    //getTranslation gets called when dropdown button is clicked
    var query = "", translation = "", myAudio = "";
    function getTranslation(source, target, text) {
        //construct query from source, target, and text
        query = "?source=" + source + "&target=" + target + "&text=" + text
        //call python backend for translation
        $.getJSON(query, function (data) {
            //get translation
            translation = data.translations[0].target;
            //print translation to DOM on success
            $("#results").html(translation);
            //and change src for audio
            $("#myAudioSrc").attr("src", function () {
                return "/static/mp3s/" + translation + ".mp3"
            });
            //get audio element
            myAudio = $("#myAudio").get(0);
            //load audio
            myAudio.load();
            //play audio
            myAudio.play();
            
        }).fail(function () {
            //throw an error if request fails
            new Error("Error: unable to get JSON")
        });
    }

    //take keys from langs object (in myLang.js )
    Object.keys(langs).forEach(function (lang, index) {
        //make a dropdown button for each language
        $("#myDropDown").append(
            "<li class='dropDown'><a href='#'>" + lang + "</a></li>"
        );
    });

    //source assumed to be english
    var source = "en", target = "", text = "";
    $(".dropDown").click(function () {
        //get target and text on button click
        target = langs[$(this).children().html()];
        text = $("#textField").val();
        //if there is text then call getTranslation
        if (target && text) {
            getTranslation(source, target, text);
        }
    });
});