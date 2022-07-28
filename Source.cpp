#include <windows.h>
#include <stdlib.h>
#include<string>
#include <iostream>
#include <fstream>
#include <cstdint>
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
using namespace std;
using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;


typedef struct  WAV_HEADER
{
    /* RIFF Chunk Descriptor */
    uint8_t         RIFF[4];        // RIFF Header Magic header
    uint32_t        ChunkSize;      // RIFF Chunk Size
    uint8_t         WAVE[4];        // WAVE Header
    /* "fmt" sub-chunk */
    uint8_t         fmt[4];         // FMT header
    uint32_t        Subchunk1Size;  // Size of the fmt chunk
    uint16_t        AudioFormat;    // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM
    uint16_t        NumOfChan;      // Number of channels 1=Mono 2=Sterio
    uint32_t        SamplesPerSec;  // Sampling Frequency in Hz
    uint32_t        bytesPerSec;    // bytes per second
    uint16_t        blockAlign;     // 2=16-bit mono, 4=16-bit stereo
    uint16_t        bitsPerSample;  // Number of bits per sample
    /* "data" sub-chunk */
    uint8_t         Subchunk2ID[4]; // "data"  string
    uint32_t        Subchunk2Size;  // Sampled data length
} wav_hdr;

// find the file size
int getFileSize(FILE* inFile)
{
    int fileSize = 0;
    fseek(inFile, 0, SEEK_END);

    fileSize = ftell(inFile);

    fseek(inFile, 0, SEEK_SET);
    return fileSize;
}

int16_t MuLaw_Decode(int8_t number)
{
    const uint16_t MULAW_BIAS = 33;
    uint8_t sign = 0, position = 0;
    int16_t decoded = 0;
    uint8_t SEG_MASK = 0xf0;
    uint8_t SEG_SHIFT = 4;
    number = ~number;

    short x = (number & 0xf0 << 3) + 0x84;
    x <<= ((unsigned)number & SEG_MASK) >> SEG_SHIFT;
    if (number & 0x80)
    {
        return 0x84 - x;
    }
    else
    {
        return x - 0x84;
    }
  /*  
    if (number & 0x80)
    {
        number &= ~(1 << 7);
        sign = -1;
    }
    position = ((number & 0xF0) >> 4) + 5;
    decoded = ((1 << position) | ((number & 0x0F) << (position - 4))
        | (1 << (position - 5))) - MULAW_BIAS;
    return (sign == 0) ? (decoded) : (-(decoded));
    */
}

int main() {
    int is_ok = EXIT_FAILURE;
    const char* fileName0 = "A_eng_f1.wav";
    const char* fileName1 = "1449183466_A_eng_f1.wav";
    const char* fileName2 = "Au8A_eng_f1.wav";
    const char* fileName3 = "decode_file.wav";
    fileName1 = fileName0;
    uint8_t waveHeader[] = {
0x52, //R
0x49, //I
0x46, //F
0x46, //F
0x2c,0xc8, 0x04,0x00, //File size
0x57, //W
0x41, //A
0x56, //V
0x45, //E
0x66,0x6d, 0x74,0x20, //FMT
0x10,0x00, 0x00,0x00, //Length of format data
0x01,0x00, //Format type -- PCM
0x01,0x00, //Number of channels
0x40,0x1f,0x00,0x00, //Sample rate
0x80,0x3e,0x00,0x00, //Cumulative data rate
0x02,0x00, //8-bit stereo/16-bit mono
0x10,0x00, //bits-per-sample
0x64,0x61, 0x74,0x61, //data chunk header
0x08,0xc8, 0x04,0x00
};


    //fileName1 = fileName0;
    

    wav_hdr wavHeader;
    int headerSize = sizeof(wav_hdr), filelength = 0;
    
    string input;
    uint8_t buffer;
    uint16_t decoded_value;

    FILE* wavFile = fopen(fileName1, "rb");
    FILE* decodeFile = fopen(fileName3, "wb");

    if (!wavFile) {
        perror("File opening failed");
        return is_ok;
    }
        if (wavFile == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", fileName1);
        return 1;
    }
    if (!decodeFile) {
        perror("File opening failed");
        return is_ok;
    }
    if (decodeFile == nullptr)
    {
        fprintf(stderr, "Unable to open wave file: %s\n", fileName3);
        return 1;
    }
    for (int i = 0; i < 44; i++) {
        buffer = waveHeader[i];
        fwrite(&buffer, sizeof(buffer), 1, decodeFile);
    }
    fseek(wavFile, 44,SEEK_SET);
    while (!feof(wavFile)) {
        fread(&buffer, sizeof(buffer), 1, wavFile);
        decoded_value = MuLaw_Decode(buffer);
        fwrite(&decoded_value, sizeof(decoded_value), 1, decodeFile);

    }
    //Read the header
    //size_t bytesRead = fread(&wavHeader, 1, headerSize, wavFile);
    //cout << "Header Read " << bytesRead << " bytes." << endl;
    //if (bytesRead > 0)
    {
        ////Read the data
        //uint16_t bytesPerSample = wavHeader.bitsPerSample / 8;      //Number     of bytes per sample
        //uint64_t numSamples = wavHeader.ChunkSize / bytesPerSample; //How many samples are in the wav file?
        //static const uint16_t BUFFER_SIZE = 4096;
        //int8_t* buffer = new int8_t[BUFFER_SIZE];
        //while ((bytesRead = fread(buffer, sizeof buffer[0], BUFFER_SIZE / (sizeof buffer[0]), wavFile)) > 0)
        //{
        //    /** DO SOMETHING WITH THE WAVE DATA HERE **/

        //    cout << "Read " << bytesRead << " bytes." << endl;
        //    cout << MuLaw_Decode(bytesRead) << endl;
        //}
        //delete [] buffer;
        //buffer = nullptr;
        //filelength = getFileSize(wavFile);

        //cout << "File is                    :" << filelength << " bytes." << endl;
        //cout << "RIFF header                :" << wavHeader.RIFF[0] << wavHeader.RIFF[1] << wavHeader.RIFF[2] << wavHeader.RIFF[3] << endl;
        //cout << "WAVE header                :" << wavHeader.WAVE[0] << wavHeader.WAVE[1] << wavHeader.WAVE[2] << wavHeader.WAVE[3] << endl;
        //cout << "FMT                        :" << wavHeader.fmt[0] << wavHeader.fmt[1] << wavHeader.fmt[2] << wavHeader.fmt[3] << endl;
        //cout << "Data size                  :" << wavHeader.ChunkSize << endl;

        //// Display the sampling Rate from the header
        //cout << "Sampling Rate              :" << wavHeader.SamplesPerSec << endl;
        //cout << "Number of bits used        :" << wavHeader.bitsPerSample << endl;
        //cout << "Number of channels         :" << wavHeader.NumOfChan << endl;
        //cout << "Number of bytes per second :" << wavHeader.bytesPerSec << endl;
        //cout << "Data length                :" << wavHeader.Subchunk2Size << endl;
        //cout << "Audio Format               :" << wavHeader.AudioFormat << endl;
        //// Audio format 1=PCM,6=mulaw,7=alaw, 257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM

        //cout << "Block align                :" << wavHeader.blockAlign << endl;
        //cout << "Data string                :" << wavHeader.Subchunk2ID[0] << wavHeader.Subchunk2ID[1] << wavHeader.Subchunk2ID[2] << wavHeader.Subchunk2ID[3] << endl;
    }

     /*bool played = PlaySound(TEXT("decode_file.wav"), NULL, SND_SYNC);
     cout << "Success or Not:" << played << endl;
 */    


    fclose(wavFile);
    fclose(decodeFile);
    
    
    return 0;

}