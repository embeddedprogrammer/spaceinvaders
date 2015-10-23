function Sound
    clc
    generateCFile();
    generateHFile();
end
function generateCFile()
    fprintf('\n//C FILE\n');
    fprintf('/*\n');
    fprintf(' * soundData.c\n');
    fprintf(' *\n');
    fprintf(' *  Created on: Oct 23, 2015\n');
    fprintf(' *      Author: Jacob White and Troy Hinckley\n');
    fprintf(' */\n\n');
    fprintf('#include "soundData.h"\n');
    files = dir('*.wav');
    for i = 1:length(files)
        name = files(i).name;
        [y, Fs] = audioread(name);
        partName = strrep(name, '.wav', '');
        fprintf('#define %s_NUMBER_OF_SAMPLES %d\n', upper(partName), length(y));
        fprintf('#define %s_SAMPLE_RATE %d\n', upper(partName), Fs);
    end
    fprintf('\n');
    for i = 1:length(files)
        name = files(i).name;
        [y, Fs] = audioread(name);
        partName = strrep(name, '.wav', '');
        scaleFactor = 128;
        printArray(partName, y*scaleFactor);
    end
    fprintf('\n');
    for i = 1:length(files)
        name = files(i).name;
        [y, Fs] = audioread(name);
        partName = strrep(name, '.wav', '');
        fprintf('const sound_t %s_sound = {%s_soundData, %s_NUMBER_OF_SAMPLES, %s_SAMPLE_RATE};\n', partName, partName, upper(partName), upper(partName));
    end    
end
function generateHFile()
    files = dir('*.wav');
    fprintf('\n//H FILE\n');
    fprintf('/*\n');
    fprintf(' * soundData.h\n');
    fprintf(' *\n');
    fprintf(' *  Created on: Oct 23, 2015\n');
    fprintf(' *      Author: Jacob White and Troy Hinckley\n');
    fprintf(' */\n\n');
    fprintf('#ifndef SOUNDDATA_H_\n');
    fprintf('#define SOUNDDATA_H_\n');
    fprintf('\n');
    fprintf('typedef struct\n');
    fprintf('{\n');
    fprintf('\tconst int* soundData;\n');
    fprintf('\tint numberOfSamples;\n');
    fprintf('\tint sampleRate;\n');
    fprintf('} sound_t;\n');
    fprintf('\n');
    for i = 1:length(files)
        name = files(i).name;
        partName = strrep(name, '.wav', '');
        fprintf('extern const sound_t %s_sound;\n', partName)
    end
    fprintf('\n');
    fprintf('#endif /* SOUNDDATA_H_ */\n');
end
function calcPrecision
    ySorted = unique(y);
    diff = ySorted(2:length(ySorted)) - ySorted(1:length(ySorted)-1);
    precision = 1/min(diff)
end
function printArray(name, a)
    lineLength = fprintf('const int %s_soundData[] = {', name);
    for i = 1:length(a)
        if lineLength >= 20000
            lineLength = 0;
            fprintf('\n\t');
        end
        if i > 1
            lineLength = lineLength + fprintf(' ');
        end
        lineLength = lineLength + fprintf('%d', a(i));
        if i < length(a)
            lineLength = lineLength + fprintf(',');
        end
    end
    fprintf('};\n')
end