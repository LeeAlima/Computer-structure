// 313467441 Lee alima

#include <stdio.h>
#include <string.h>

void taskOne(char *sourceFile, char *targetFile,int flagSwap);
void tasksTwoAndThree(char *sourceFile,char *targetFile
        , char *typeOfSource, char *typeOfTarget,int flagOfSwap);
void changeGeneral(char *sourceFile, char *targetFile, char *sourceFileType,
                   char *targetFileType, int swapFlag, int saveNumOfEnters);
void changeFromWin(char *sourceFile,char *targetFile,
                   char *targetFileType,int swapFlag);
void makeArray(char array[],int unixFlag, int littleEndianFlag);
int checkLegalTexts(char *sourceFile, char *targetFile);
int checkFirstFlags(char *sourceFileType, char *targetFileType);
const int numberOfReading = 1;
const char null = '\000';

/**
 * the main method checks the number of args and if they
 * are llegal and calls the right method.
 * @param argc - number of args
 * @param argv - strings
 * @return number
 */
int main(int argc, char **argv)
{
    if (argc < 3) {
        return 0;
    } // check if the first args are on the type of files names
    if (!checkLegalTexts(argv[1], argv[2])){
        return 0;
    }
    if (argc == 3){
        taskOne(argv[1], argv[2],0);
    }
    // if there are more than 4 args and the flags suits
    if (argc > 4 && checkFirstFlags(argv[3],argv[4])){
        if (argc == 5) {
            tasksTwoAndThree(argv[1],argv[2],argv[3],argv[4],0);
        }
        if (argc == 6) {
            if (!strcmp(argv[5],"-keep")){
                tasksTwoAndThree(argv[1],argv[2],argv[3],argv[4],0);
                return 0;
            }
            if (!strcmp(argv[5],"-swap")){
                tasksTwoAndThree(argv[1],argv[2],argv[3],argv[4],1);
            }
        }
    }
}

/**
 * this method only copy file's from a kind to the same kind.
 * I opened 2 files, then I read the file with 2 bytes's at a time,
 * I checked if they must be swaped and then I wrote them into the file.
 * @param sourceFile - as the name of the source file.
 * @param targetFile - as the name of the target file.
 * @param flagSwap - 1 if flag is -swap, 0 otherwise.
 */
void taskOne(char *sourceFile, char *targetFile,int flagSwap) {
    FILE * openFile = fopen(sourceFile,"rb");
    // if for some reason the file couldn't be opened, than exit.
    if (openFile == NULL){
        return;
    }
    // if for some reasen the file couldn't be opened, than exit.
    FILE * target = fopen(targetFile, "wb");
    if (target ==  NULL){
        // close the first file
        fclose(openFile);
        return;
    }
    char buffer[2],transform[2];
    // read one char, in utf - 16 , the meaning is 2 bytes.
    // save them at a buffer and copy it to the target file
    while (fread(buffer, sizeof(buffer),numberOfReading , openFile)){
        // if the flag is 1, swap the bytes
        if (flagSwap == 1){
            transform[0] = buffer[1];
            transform[1] = buffer[0];
            fwrite(transform, sizeof(transform),numberOfReading,target);
        }
        else {
            fwrite(buffer, sizeof(buffer),numberOfReading,target);
        }
    }
    // when the reading && writing stops, close the files.
    fclose(openFile);
    fclose(target);
}

/**
 * this method calls the right method with the right parameters and flags.
 * @param sourceFile - as the name of the source file.
 * @param targetFile - as the name of the target file.
 * @param typeOfSource - as the type of the source file.
 * @param typeOfTarget - as the type of the target file.
 * @param flagOfSwap - 1 if the bytes must be swaped, 0 otherwise.
 */
void tasksTwoAndThree(char *sourceFile,char *targetFile, char *typeOfSource
        , char *typeOfTarget,int flagOfSwap){
    // the same kind of types
    if (!strcmp(typeOfSource,typeOfTarget)){
        taskOne(sourceFile,targetFile,flagOfSwap);
    } // unix -> mac or mac-> unix
    else if ((!strcmp(typeOfSource,"-unix") && !strcmp(typeOfTarget,"-mac")) ||
             (!strcmp(typeOfSource,"-mac") && !strcmp(typeOfTarget,"-unix"))){
        changeGeneral(sourceFile, targetFile, typeOfSource,
                      typeOfTarget, flagOfSwap, 1);
    } // win -> mac or win ->unix
    else if ((!strcmp(typeOfSource,"-win") && !strcmp(typeOfTarget,"-mac"))
             ||(!strcmp(typeOfSource,"-win")&& !strcmp(typeOfTarget,"-unix"))){
        changeFromWin(sourceFile,targetFile,typeOfTarget,flagOfSwap);
    } // unix->win or mac->win
    else if ((!strcmp(typeOfSource,"-unix") && !strcmp(typeOfTarget,"-win"))
             || (!strcmp(typeOfSource,"-mac")&& !strcmp(typeOfTarget,"-win"))){
        changeGeneral(sourceFile, targetFile, typeOfSource,
                      typeOfTarget, flagOfSwap, 0);
    } else {
        return;
    }
}

/**
 * this method change a file from win format to unix or mac,
 * by the parameters I gave the method I know what enter must be written.
 * I always read 2 bytes and I save the previous two bytes, in this way
 * I know if and "\r\n" appeared in the source file and the asked enter
 * must be written or not (i checked for both endians).
 * I also checked if the text was written in little or big endian,
 * and wrote the suit char.
 * @param sourceFile - as the name of the source file.
 * @param targetFile - as the name of the target file.
 * @param targetFileType - as the type of the target file.
 * @param swapFlag - 1 if the bytes must be swapped, 0 otherwise.
 */
void changeFromWin(char *sourceFile,char *targetFile,
                   char *targetFileType,int swapFlag){
    // open a file, return if couldn't open it.
    FILE * openFile = fopen(sourceFile,"rb");
    if (openFile == NULL){
        return;
    }
    // open the second file, close the first file and return if failed
    FILE * target = fopen(targetFile, "wb");
    if (target ==  NULL){
        fclose(openFile);
        return;
    }
    char buffer[2] ,prevBuf[2] ,transformL[2],unixEnterL[2],macEnterL[2];
    char unixEnterB[2],macEnterB[2],toWriteB[2],toWriteL[2],transformB[2];
    // "mark" prevBuf toWrite know if it's the first time of reading a buffer
    prevBuf[0] = null;
    prevBuf[1] = null;
    if (!strcmp(targetFileType,"-unix")){
        makeArray(toWriteL,1,1);
        makeArray(toWriteB,1,0);

    }
    if (!strcmp(targetFileType,"-mac")){
        makeArray(toWriteL,0,1);
        makeArray(toWriteB,0,0);

    }
    makeArray(unixEnterL,1,1);
    makeArray(macEnterL,0,1);
    makeArray(unixEnterB,1,0);
    makeArray(macEnterB,0,0);
    int cantWriteFlag = 0;
    while (fread(buffer, sizeof(buffer),numberOfReading , openFile)){
        transformL[0] = toWriteL[1];
        transformL[1] = toWriteL[0];
        transformB[0] = toWriteB[1];
        transformB[1] = toWriteB[0];
        // if it's not the first time of reading
        if (!(prevBuf[0]== null && prevBuf[1] == null)){
            // if an enter in windows was read
            // (checking both Big and little endian)
            if ((buffer[0] == unixEnterL[0] && buffer[1] == unixEnterL[1]
                 && prevBuf[0] == macEnterL[0]  && prevBuf[1]== macEnterL[1])
                    || (buffer[0] == unixEnterB[0] &&
                    buffer[1] == unixEnterB[1] && prevBuf[0] == macEnterB[0]
                        && prevBuf[1] == macEnterB[1])){
                if (swapFlag == 0 ) { // no need toWrite swap
                    if (buffer[0] == null){ //shows the file is in big endian
                        fwrite(toWriteB, sizeof(toWriteB)
                                ,numberOfReading,target);
                    } else {
                        fwrite(toWriteL, sizeof(toWriteL)
                                ,numberOfReading,target);
                    }
                } else { // need toWrite swap
                    if (buffer[0]== null){//write the transform in Big endian
                        fwrite(transformB,sizeof(transformB),
                               numberOfReading,target);
                    } else {
                        fwrite(transformL,sizeof(transformL),
                               numberOfReading,target);
                    }
                }
                cantWriteFlag = 1;
            } else { // if an enter wasn't read
                if (!cantWriteFlag) {
                    // write in the right mode by the flag
                    if (swapFlag == 0) {
                        fwrite(prevBuf,sizeof(prevBuf),numberOfReading,target);
                    } else { // swap the bytes
                        char traPrev[2];
                        traPrev[0] = prevBuf[1];
                        traPrev[1]= prevBuf[0];
                        fwrite(traPrev,sizeof(traPrev),numberOfReading,target);
                    }
                } else { // make it possible toWrite write in the next turn
                    cantWriteFlag = 0;
                }
            }
        } // reset prevBuf with the vals of buffer
        prevBuf[0]=buffer[0];
        prevBuf[1]=buffer[1];
    } // close the files
    fclose(openFile);
    fclose(target);
}

/**
 * this method works more generically, with the parameters I gave the method
 * I know what kind of enter to search and what enter to write.
 * I added the saveNumOfEnters flag in order to easier know if
 * the target file is win and If I have to write more than just
 * 2 bytes in order to write an enter.
 * Every time I wrote an enter, I checked all of the flags and wrote
 * the suitable enter.
 * I also checked if the text was written in little or big endian,
 * and wrote according to this information.
 * @param sourceFile - as the name of the source file.
 * @param targetFile - as the name of the target file.
 * @param sourceFileType - as the type of the source file.
 * @param targetFileType - as the type of the target file
 * @param swapFlag - 1 if the bytes must be swap and 0 otherwise.
 * @param saveNumOfEnters - 0 if the target is win and 1 otherwise.
 */
void changeGeneral(char *sourceFile, char *targetFile, char *sourceFileType,
                   char *targetFileType, int swapFlag, int saveNumOfEnters){
    FILE * openFile = fopen(sourceFile,"rb");
    // if for some reason the file couldn't be opened, than exit.
    if (openFile == NULL){
        return;
    }
    // if for some reason the file couldn't be opened, than exit.
    FILE * target = fopen(targetFile, "wb");
    if (target ==  NULL){
        // close the firstReadFlag file
        fclose(openFile);
        return;
    }
    // littleEndianTextFlag symbolizes if the file was
    // written in little endian or not.
    int littleEndianTextFlag,toUnixEnterFlag=0;
    char fromTextL[2],toRLittle[2],toRBig[2],toNLittle[2],secondTypeB[2];
    char toNBig[2],buffer[2]/*,secondType[2]*/,fromTextB[2],secondTypeL[2];
    // reset the array's with the right flags
    makeArray(toRLittle,0,1);
    makeArray(toRBig,0,0);
    makeArray(toNLittle,1,1);
    makeArray(toNBig,1,0);
    // fill the arrays with the right values based on parameters
    if (!strcmp(sourceFileType,"-unix")){
        makeArray(fromTextL,1,1);
        makeArray(fromTextB,1,0);
    } else {
        makeArray(fromTextL,0,1);
        makeArray(fromTextB,0,0);
    }
    if (!strcmp(targetFileType,"-unix")){
        makeArray(secondTypeB,1,0);
        makeArray(secondTypeL,1,1);
        toUnixEnterFlag = 1;
    } else {
        makeArray(secondTypeB,0,0);
        makeArray(secondTypeL,0,1);
    }
    while (fread(buffer, sizeof(buffer),numberOfReading , openFile)){
        if(swapFlag == 0){ // if enter was found
            if ((buffer[0] == fromTextL[0] && buffer[1] == fromTextL[1])||
                    (buffer[0] == fromTextB[0] && buffer[1] == fromTextB[1])) {
                if (!saveNumOfEnters){ // write "\r\n" in the right form
                    if (buffer[0] != null){ // little endian
                        fwrite(toRLittle, sizeof(toRLittle),
                               numberOfReading,target);
                        fwrite(toNLittle, sizeof(toNLittle),
                               numberOfReading,target);
                    } else { // big endian
                        fwrite(toRBig, sizeof(toRBig),
                               numberOfReading,target);
                        fwrite(toNBig, sizeof(toNBig),
                               numberOfReading,target);
                    }
                } else { // write the enter as should by the second type
                    if (buffer[0] != null) { //little endian
                        fwrite(secondTypeL,sizeof(secondTypeL)
                                ,numberOfReading,target);
                    } else {
                        fwrite(secondTypeB,sizeof(secondTypeB)
                                ,numberOfReading,target);
                    }
                }
            } else { // write the buffer into the file with no change
                fwrite(buffer, sizeof(buffer),numberOfReading,target);
            }
        } else { // if the bytes must be swapped
            if ((buffer[0] == fromTextL[0] && buffer[1] == fromTextL[1])||
            (buffer[0] == fromTextB[0] && buffer[1] == fromTextB[1])){
                char traUnixEnter[2], tranMacEnter[2];
                //for little endian make littleEndianTextFlag 1
                if (buffer[0] != null){
                    littleEndianTextFlag = 1;
                }
                else {
                    littleEndianTextFlag =0;
                }
                // reset the array's with swapped bytes
                makeArray(traUnixEnter,1,!littleEndianTextFlag);
                makeArray(tranMacEnter,0,!littleEndianTextFlag);
                // if the target file is win
                if (!saveNumOfEnters){
                    fwrite(tranMacEnter, sizeof(tranMacEnter),
                           numberOfReading,target);
                    fwrite(traUnixEnter, sizeof(traUnixEnter),
                           numberOfReading,target);
                } else{
                    char swappedEnter[2];
                    makeArray(swappedEnter,toUnixEnterFlag,
                              !littleEndianTextFlag);
                    fwrite(swappedEnter, sizeof(swappedEnter),
                           numberOfReading,target);
                }
            } else { // swap the buffer
                char transpose [2];
                transpose[0] = buffer[1];
                transpose[1] = buffer[0];
                fwrite(transpose, sizeof(transpose),numberOfReading,target);
            }
        }
    }
    // when the reading && writing stops, close the files.
    fclose(openFile);
    fclose(target);
}

/**
 * this method gets an array and reset it by the flag's information.
 * @param array - as the array to reset.
 * @param unixFlag - 1 if the enter should be '\n', 0 if it should
 * be '\r'.
 * @param littleEndianFlag - 1 if the way the array should be reset
 * must be on little or big endian.
 */
void makeArray(char array[],int unixFlag, int littleEndianFlag){
    char toWrite = '\r';
    if (unixFlag == 1){
        toWrite = '\n';
    }
    if (littleEndianFlag){
        array[0] = toWrite;
        array[1] = null;
    } else {
        array[1] = toWrite;
        array[0] = null;
    }
}

/**
 * this method checks if the 2 first vals are on the form of files
 * by finding "." in there names.
 * @param sourceFile - as the source file name
 * @param targetFile - as the target file name
 * @return 1 - if they both legal and 0 if not.
 */
int checkLegalTexts(char *sourceFile, char *targetFile){
    char *sourcePointer = strstr(sourceFile,".");
    char *targetPointer = strstr(targetFile,".");
    if (sourcePointer && targetPointer){
        return 1;
    }
    return 0;
}

/**
 * this method checks the first 2 flags of the args if exist.
 * @param sourceFileType - as the name of the source file
 * @param targetFileType - as the name of the target file
 * @return 1 if the flags are legal and 0 otherwise
 */
int checkFirstFlags(char *sourceFileType, char *targetFileType){
    int rightSourceType = 0;
    int rightTargetType = 0;
    if (!strcmp(sourceFileType,"-unix") || !strcmp(sourceFileType,"-mac") ||
            !strcmp(sourceFileType,"-win")){
        rightSourceType = 1;
    }
    if (!strcmp(targetFileType,"-unix") || !strcmp(targetFileType,"-mac") ||
        !strcmp(targetFileType,"-win")){
        rightTargetType = 1;
    }
    if (rightSourceType && rightTargetType){
        return 1;
    }
    return 0;
}