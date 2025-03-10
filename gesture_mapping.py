import spacy
from pyfirmata import Arduino, util
import time


nlp = spacy.load("en_core_web_sm")
board = Arduino('COM10')

servo_pins = {1: board.get_pin('d:8:s'), 2: board.get_pin('d:11:s'),  3: board.get_pin('d:10:s'),  4: board.get_pin('d:7:s'),  5: board.get_pin('d:6:s'), 6: board.get_pin('d:5:s'),7: board.get_pin('d:9:s'), 8: board.get_pin('d:4:s')  }

def move_servo(servo_id, angle):
    """
    Move the specified servo t
    
    
    o the given angle.
    servo_id: The ID of the servo motor (1 to 8).
    angle: Angle to move to (0 to 180 degrees).
    """
    if servo_id in servo_pins and 0 <= angle <= 180:
       servo_pins[servo_id].write(angle)
       print(f"Servo {servo_id} moved to {angle} degrees on pin .")
       time.sleep(0.1)  # Allow the servo time to reach the position
    else:
        print(f"Invalid servo ID ({servo_id}) or angle ({angle})!")


def preprocessing(sentence):
    doc = nlp(sentence.lower())
    # Keep essential parts of speech: pronouns, nouns, verbs
    asl_words = []
    for token in doc:
        if token.pos_ in ['PRON', 'NOUN', 'VERB']:
            if token.pos_ == 'VERB':
                # Use lemmatized form for verbs
                asl_words.append(token.lemma_)
            else:
                asl_words.append(token.text)
    # Join the words into a concise ASL-like sentence
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


def index(angle):
    move_servo(1,angle)

def middle(angle):
    move_servo(2,angle)

def ring(angle):
    move_servo(3,angle)

def pinky(angle):
    move_servo(4,angle)

def thumb(angle):
    move_servo(5,angle)

def wrist(angle):
    move_servo(6,angle)

def seperate(value):
    if value:
        move_servo(7,30)

def ASL_letter(angles):
    index(angles[0])
    middle(angles[1])
    ring(angles[2])
    pinky(angles[3])
    thumb(angles[4])
    wrist(angles[5])
    seperate(angles[6])

    

mapping={
    '': [0, 0, 0, 0, 0,0,0],
    'a': [1, 1, 1, 1, 1,1,1],
    'b': [2, 2, 2, 2, 2,1,1],
    'c': [3, 3, 3, 3, 3,1,1],
    'd': [4, 4, 4, 4, 4,1,1],
    'e': [5, 5, 5, 5, 5,1,1],
    'f': [6, 6, 6, 6, 6,1,1],
    'g': [7, 7, 7, 7, 7,1,1],
    'h': [8, 8, 8, 8, 8,1,1],
    'i': [9, 9, 9, 9, 9,1,1],
    'j': [10, 10, 10, 10, 10,1,1],
    'k': [11, 11, 11, 11, 11,1,1],
    'l': [12, 12, 12, 12, 12,1,1],
    'm': [13, 13, 13, 13, 13,1,1],
    'n': [14, 14, 14, 14, 14,1,1],
    'o': [15, 15, 15, 15, 15,1,1],
    'p': [16, 16, 16, 16, 16,1,1],
    'q': [17, 17, 17, 17, 17,1,1],
    'r': [18, 18, 18, 18, 18,1,1],
    's': [19, 19, 19, 19, 19,1,1],
    't': [20, 20, 20, 20, 20,1,1],
    'u': [21, 21, 21, 21, 21,1,1],
    'v': [22, 22, 22, 22, 22,1,1],
    'w': [23, 23, 23, 23, 23,1,1],
    'x': [24, 24, 24, 24, 24,1,1],
    'y': [25, 25, 25, 25, 25,1,1],
    'z': [26, 26, 26, 26, 26,1,1]
}

sentence = "I am going to the store to buy some milk."
preprocessed_sentence=preprocessing(sentence)
x=tokens(preprocessed_sentence)
for i in x:
    ASL_letter(mapping[i])
    #time.sleep(2)



