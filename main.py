import spacy
import time
import en_core_web_sm
from flask import Flask, request
from flask_cors import CORS, cross_origin
import json

en_core_web_sm.load()
nlp = spacy.load("en_core_web_sm")
app = Flask(__name__)

CORS(app, supports_credentials=True, origins="*")
app.config['Headers'] = 'Access-Control-Allow-Origin'
app.config['CORS_HEADERS'] = 'Content-Type'
app.config['CONTENT_TYPE'] = 'multipart/form-data'

import serial
import time


arduino = serial.Serial(port='COM8', baudrate=115200, timeout=0.1)
def write_read(x):
    time.sleep(0.1)
    cleaned = x.lower().replace(" ", "")
    chars = list(cleaned)
    
    arduino.flushInput()
    arduino.write(','.join(chars).encode('utf-8') + b'\n')
    return arduino.readline().decode().strip()


def preprocessing(sentence):
    doc = nlp(sentence.lower())

    asl_words = []
    for token in doc:
        if token.pos_ in ['PRON', 'NOUN', 'VERB']:
            if token.pos_ == 'VERB':
                asl_words.append(token.lemma_)
            else:
                asl_words.append(token.text)

    asl_ordered = " ".join(asl_words)
    return asl_ordered

def tokens(sentence):
    common_words=[]
    tokens=[]
    words= sentence.split(' ')

    for word in words:
        if word in common_words:
            tokens.append(word)
        else:
            for letter in word:
                tokens.append(letter)

        tokens.append('')

    return tokens

@app.route("/process", methods = ['POST','GET'])
@cross_origin()
def process():

    sentence = request.json['text']
    preprocessed_sentence=preprocessing(sentence)
    
    sentence_tokens = tokens(preprocessed_sentence)
    formatted_tokens = ""
    arduino_tokens = []

    for token in sentence_tokens:
        if token == "":
            formatted_tokens += " "
        else: formatted_tokens += token


    for token in sentence_tokens:
        if token == "":
            continue
        arduino_tokens.append(token)


    val = write_read(",".join(arduino_tokens))
    print(val)
    return json.dumps((sentence_tokens, formatted_tokens))

app.run()

