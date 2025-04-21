const darkModeToggle = document.getElementById('darkModeToggle');

const savedDarkMode = localStorage.getItem('darkMode');
if (savedDarkMode === 'true') {
    document.body.classList.add('dark-mode');
    darkModeToggle.checked = true;
}

darkModeToggle.addEventListener('change', () => {
    document.body.classList.toggle('dark-mode');

    localStorage.setItem('darkMode', darkModeToggle.checked);
});

document.getElementById('translatorForm').addEventListener('submit', async function(e) {
    console.log("Form submitted");
    e.preventDefault();
    const inputText = document.getElementById('inputText').value;
    
    let preprocessedText = await fetch("http://127.0.0.1:5000/process", {method:"POST", headers: new Headers({'content-type': 'application/json'}), body: JSON.stringify({text : inputText})})
    preprocessedText = await preprocessedText.json()

    document.getElementById('preprocessedOutput').textContent = preprocessedText[1];
    
    displayTokens(preprocessedText[0]);
    
    animateFingers(preprocessedText[0]);
});

function displayTokens(tokens) {
    const tokensDisplay = document.getElementById('tokensOutput');
    tokensDisplay.innerHTML = tokens.map(token => 
        token === '' ? '&nbsp;<span class="badge bg-secondary">space</span>&nbsp;' : 
        `<span class="badge bg-primary">${token}</span>`
    ).join(' ');
}

function animateFingers(tokens) {

    let currentIndex = 0;
    const letterDisplay = document.getElementById('currentLetter');

    if (window.animationInterval) {
        clearInterval(window.animationInterval);
    }

    function updateFingers(letter) {
        letterDisplay.textContent = letter === '' ? 'Space' : letter.toUpperCase();
    }

    updateFingers(tokens[0]);

    window.animationInterval = setInterval(() => {
        currentIndex++;
        if (currentIndex >= tokens.length) {
            clearInterval(window.animationInterval);
            setTimeout(() => {

                letterDisplay.textContent = "Done";
                
                document.querySelectorAll('.finger').forEach(finger => {
                    finger.style.transform = 'rotate(0deg)';

                });

            }, 1000);


            return;
        }   
        updateFingers(tokens[currentIndex]);
    }, 1000);

}

const GEMINI_API_KEY = "ADD_KEY_DONT_FORGET_TO_ADD_KEY"; // Replace with your actual API key
if (GEMINI_API_KEY === "ADD_KEY_DONT_FORGET_TO_ADD_KEY") {
    console.error(`GEMINI_API_KEY is not set. Please replace it with your actual" API key.`);
    }
const UPLOAD_API_URL = `https://generativelanguage.googleapis.com/upload/v1beta/files?key=${GEMINI_API_KEY}`;
const MODEL_ID = "gemini-2.0-flash-lite"; 
const GENERATE_CONTENT_API = "generateContent";
const GENERATE_API_URL = `https://generativelanguage.googleapis.com/v1beta/models/${MODEL_ID}:${GENERATE_CONTENT_API}?key=${GEMINI_API_KEY}`;

const startRecordBtn = document.getElementById('startRecordBtn');
const stopRecordBtn = document.getElementById('stopRecordBtn');
const audioStatus = document.getElementById('audioStatus');
const audioPlayback = document.getElementById('audioPlayback');
const transcriptionOutput = document.getElementById('transcriptionOutput');

let mediaRecorder;
let audioChunks = [];
let recordingStream;

const recordingMimeType = 'audio/webm; codecs=opus';
function updateAudioStatus(message) {
    audioStatus.textContent = `Status: ${message}`;
    console.log(`Audio Status: ${message}`);
}


startRecordBtn.addEventListener('click', async () => {
    if (!GEMINI_API_KEY || GEMINI_API_KEY === "YOUR_GEMINI_API_KEY_HERE") {
        updateAudioStatus("API Key Missing! Please edit the script.");
        alert("Error: GEMINI_API_KEY is not set in the script. Please edit the HTML file.");
        return;
    }
    updateAudioStatus("Requesting microphone access...");
    transcriptionOutput.textContent = 'Transcription will appear here...'; 
    audioPlayback.removeAttribute('src'); 

    try {
        recordingStream = await navigator.mediaDevices.getUserMedia({ audio: true });
        updateAudioStatus("Microphone access granted.");

        mediaRecorder = new MediaRecorder(recordingStream, { mimeType: recordingMimeType });
        audioChunks = []; 

        mediaRecorder.ondataavailable = (event) => {
            if (event.data.size > 0) {
                audioChunks.push(event.data);
            }
        };

        mediaRecorder.onstop = async () => {
            updateAudioStatus("Recording stopped. Processing audio...");
            if (audioChunks.length === 0) {
                    updateAudioStatus("Error: No audio data recorded.");
                    return;
            }

            const audioBlob = new Blob(audioChunks, { type: recordingMimeType });
            audioPlayback.src = URL.createObjectURL(audioBlob);

            try {
                updateAudioStatus("Uploading audio file...");
                const fileUri = await uploadFileToGemini(audioBlob);
                updateAudioStatus(`File uploaded. URI: ${fileUri}. Transcribing...`);
                await transcribeAudio(fileUri, audioBlob.type);
                updateAudioStatus("Transcription complete.");
            } catch (error) {
                console.error("Error during upload/transcription:", error);
                updateAudioStatus(`Error: ${error.message}`);
                transcriptionOutput.textContent = `Error: ${error.message}`;
            } finally {
                if (recordingStream) {
                        recordingStream.getTracks().forEach(track => track.stop());
                        recordingStream = null;
                        console.log("Microphone stream stopped.");
                    }
            }
        };

            mediaRecorder.onerror = (event) => {
                console.error('MediaRecorder error:', event.error);
                updateAudioStatus(`Recording Error: ${event.error.message}`);
                stopRecording();
            };

        mediaRecorder.start();
        updateAudioStatus("Recording...");
        startRecordBtn.disabled = true;
        stopRecordBtn.disabled = false;

    } catch (err) {
        console.error("Error starting recording:", err);
        updateAudioStatus(`Error: ${err.message}`);
        startRecordBtn.disabled = false;
        stopRecordBtn.disabled = true;
    }
});

stopRecordBtn.addEventListener('click', stopRecording);

function stopRecording() {
        if (mediaRecorder && mediaRecorder.state === "recording") {
        mediaRecorder.stop();
        console.log("MediaRecorder stop called.");
    } else {
        console.log("MediaRecorder not recording or already stopped.");
            startRecordBtn.disabled = false;
            stopRecordBtn.disabled = true;
            if (recordingStream) {
                recordingStream.getTracks().forEach(track => track.stop());
                recordingStream = null;
                console.log("Microphone stream stopped manually.");
            }
    }
}


async function uploadFileToGemini(fileBlob) {
    const fileSize = fileBlob.size;
    const fileMimeType = fileBlob.type;
    const displayName = `web-recording-${new Date().toISOString()}.ogg`;

    console.log("Starting resumable upload...");
    let uploadUrl;
    try {
        const startResponse = await fetch(UPLOAD_API_URL, {
            method: 'POST',
            headers: {
                'X-Goog-Upload-Protocol': 'resumable',
                'X-Goog-Upload-Command': 'start',
                'X-Goog-Upload-Header-Content-Length': fileSize.toString(),
                'X-Goog-Upload-Header-Content-Type': fileMimeType,
                'X-Goog-Upload-Offset': '0',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                file: { display_name: displayName }
            })
        });

        if (!startResponse.ok || startResponse.status !== 200) {
            const errorText = await startResponse.text();
            throw new Error(`Failed to start upload: ${startResponse.status} - ${errorText}`);
        }

        uploadUrl = startResponse.headers.get('x-goog-upload-url');
        if (!uploadUrl) {
            throw new Error('Upload URL not found in start response headers.');
        }
        console.log("Obtained Upload URL:", uploadUrl);

    } catch (error) {
        console.error("Error starting upload:", error);
        throw new Error(`Upload Start Failed: ${error.message}`); 
    }
    try {
        const uploadResponse = await fetch(uploadUrl, {
            method: 'POST', 
            headers: {
                'Content-Length': fileSize.toString(),
                'X-Goog-Upload-Command': 'upload, finalize',
                'X-Goog-Upload-Offset': '0',
                'X-Goog-Upload-Protocol': 'resumable',
                'X-Goog-Upload-File-Name': displayName,
                'X-Goog-Upload-Header-Content-Type': fileMimeType,
            },
            body: fileBlob
        });

        if (!uploadResponse.ok) {
            const errorText = await uploadResponse.text();
            throw new Error(`Failed to upload file bytes: ${uploadResponse.status} - ${errorText}`);
        }

        const fileInfo = await uploadResponse.json();
        console.log("File Upload Response:", fileInfo);

        if (!fileInfo.file || !fileInfo.file.uri) {
            throw new Error("File URI not found in finalize response.");
        }
        return fileInfo.file.uri;

    } catch (error) {
        console.error("Error uploading/finalizing:", error);
        throw new Error(`Upload Failed: ${error.message}`); 
    }
}

async function transcribeAudio(fileUri, fileMimeType) {
    transcriptionOutput.textContent = 'Processing transcription...';

    const requestBody = {
        "contents": [{
            "role": "user",
            "parts": [
                {
                    "fileData": {
                        "mimeType": fileMimeType,
                        "fileUri": fileUri
                    }
                },
            ]
        }],
        "systemInstruction": {
            "parts": [{
                "text": "You are a voice-to-text transcriber. You are to only do that. Do not reply with anything other than the audio provided in the given file, every single time. Nothing else is allowed. Only the transcribed audio."
            }]
        },
        "generationConfig": {
            "responseMimeType": "text/plain"
        }
    };

    try {
        const response = await fetch(GENERATE_API_URL, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(requestBody)
        });

        if (!response.ok) {
            const errorText = await response.text();
            throw new Error(`Gemini API Error: ${response.status} - ${errorText}`);
        }

        const result = await response.json();
        console.log("Complete transcription response:", result);
        
        const transcription = result.candidates?.[0]?.content?.parts?.[0]?.text || "No transcription found in response";
        
        transcriptionOutput.textContent = transcription;
        const inputText = document.getElementById('inputText')
        inputText.value = transcription;
        document.getElementById('translatorForm').dispatchEvent(new Event('submit'));
        console.log("Transcription:", transcription);
        
    } catch (error) {
        console.error("Error during transcription API call:", error);
        transcriptionOutput.textContent = `Error: ${error.message}`;
        throw error;
    }
}

updateAudioStatus("Idle. Press Start Recording.");