#include <SDHCI.h>
#include <Audio.h>

#include <arch/board/board.h>

#define RECORD_FILE_NAME "25.wav"

SDClass theSD;
AudioClass *theAudio;

File myFile;

bool ErrEnd = false;

/**
 * @brief Audio attention callback
 *
 * When audio internal error occurs, this function will be called back.
 */

static void audio_attention_cb(const ErrorAttentionParam *atprm)
{
  puts("Attention!");
  
  if (atprm->error_code >= AS_ATTENTION_CODE_WARNING)
    {
      ErrEnd = true;
   }
}

/**
 * @brief Setup recording of wav stream to file
 *
 * Select input device as microphone <br>
 * Initialize filetype to stereo wav with 48 Kb/s sampling rate <br>
 * Open RECORD_FILE_NAME file in write mode
 */

/* Sampling rate
 * Set 16000 or 48000
 */

static const uint32_t recoding_sampling_rate = 48000;

/* Number of input channels
 * Set either 1, 2, or 4.
 */

static const uint8_t  recoding_cannel_number = 4;

/* Audio bit depth
 * Set 16 or 24
 */

static const uint8_t  recoding_bit_length = 16;

/* Recording time[second] */

static const uint32_t recoding_time = 20;

/* Bytes per second */

static const int32_t recoding_byte_per_second = recoding_sampling_rate *
                                                recoding_cannel_number *
                                                recoding_bit_length / 8;

/* Total recording size */

static const int32_t recoding_size = recoding_byte_per_second * recoding_time;

void setup()
{

  Serial.begin(9600);

  /* Initialize SD */
  while (!theSD.begin())
    {
      /* wait until SD card is mounted. */
      Serial.println("Insert SD card.");
    }

  theAudio = AudioClass::getInstance();

  theAudio->begin(audio_attention_cb);

  puts("initialization Audio Library");

  /* Select input device as microphone */
  theAudio->setRecorderMode(AS_SETRECDR_STS_INPUTDEVICE_MIC);//指定麦克风为输入设备

  /* Search for WAVDEC codec in "/mnt/sd0/BIN" directory */
  theAudio->initRecorder(AS_CODECTYPE_WAV,
                         "/BIN",
                         recoding_sampling_rate,
                         recoding_bit_length,
                         recoding_cannel_number);
  puts("Init Recorder!");

  /* Open file for data write on SD card */

  if (theSD.exists(RECORD_FILE_NAME))
    {
      printf("Remove existing file [%s].\n", RECORD_FILE_NAME);
      theSD.remove(RECORD_FILE_NAME);
    }

  myFile = theSD.open(RECORD_FILE_NAME, FILE_WRITE);
  /* Verify file open */
  if (!myFile)
    {
      printf("File open error\n");
      exit(1);
    }

  printf("Open! [%s]\n", RECORD_FILE_NAME);

  theAudio->writeWavHeader(myFile);
  puts("Write Header!");

  Serial.println("Start Recording - ");
  theAudio->startRecorder();
  puts("Recording Start!");

}

void loop() 
{
  
  err_t err;
  /* recording end condition */
  if (theAudio->getRecordingSize() > recoding_size)
    {
      theAudio->stopRecorder();
      sleep(1);
      err = theAudio->readFrames(myFile);

      goto exitRecording;
    }


  /* Read frames to record in file */
  err = theAudio->readFrames(myFile);

  if (err != AUDIOLIB_ECODE_OK)
    {
      printf("File End! =%d\n",err);
      theAudio->stopRecorder();
      goto exitRecording;
      Serial.println("Stop Recording -");
    }

  if (ErrEnd)
    {
      printf("Error End\n");
      theAudio->stopRecorder();
      goto exitRecording;
    }

  /* This sleep is adjusted by the time to write the audio stream file.
   * Please adjust in according with the processing contents
   * being processed at the same time by Application.
   *
   * The usleep() function suspends execution of the calling thread for usec
   * microseconds. But the timer resolution depends on the OS system tick time
   * which is 10 milliseconds (10,000 microseconds) by default. Therefore,
   * it will sleep for a longer time than the time requested here.
   */

//  usleep(10000);

  return;

exitRecording:

  theAudio->closeOutputFile(myFile);
  myFile.close();
  
  theAudio->setReadyMode();
  theAudio->end();
  
  puts("End Recording");
  exit(1);

}
