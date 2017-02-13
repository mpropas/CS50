Welcome to my CS50 Final Project

//An overview video of this project can be seen at https://www.youtube.com/watch?v=WC5lKTknl0Y&feature=youtu.be

This is a small full stack application that brings together many of the languages and concepts from the course. It features:

1) The html page uses bootstrap with a few lines of custom css to make things work better
2) The front end runs a small JQuery based script to populate the DOM when the page is first loaded and to handle click events
3) When a language and text exists and a click event occurs:
  - First a $.getJSON request is sent to my python backend which implements the standard google text to text API
  - Next the python server will use the gTTS library to download an mp3 file of the translated text in the target language
  - JQuery will then change the DOM's audio element to the current translation and play the file
  
  Basically a poor man's google translate with a lot of room for expansion
