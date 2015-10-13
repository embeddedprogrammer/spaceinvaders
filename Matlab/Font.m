function Font()
    i = imread('Font.png');
    fontChars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
%     for x=1:(26*2+10)
%         name = sprintf('character_%c_5x8', fontChars(x));
%         generateBitmap(name, i(1:8, (1:5)+(x-1)*6));
%     end
    fprintf('static const int* characters[26*2+10] characters = {');
    for x=1:(26*2+10)
        fprintf('\tcharacter_%c_5x8,\n', fontChars(x));
    end
    fprintf('};');    
end
function generateBitmap(name, i)
    fprintf('static const int %s[] = {\n', name);
    printArray(i(1, 1:size(i,2)));
    for y = 2:size(i,1)
        fprintf(',\n');
        printArray(i(y, 1:size(i,2)));
    end
    fprintf('\n};\n\n');
end
function printArray(a)
    fprintf('\tpackword%d(', size(a, 2));
    printPixel(a(1));
    for x = 2:size(a, 2)
        fprintf(', ');
        printPixel(a(x));
    end
    fprintf(')');
end
function printPixel(p)
    fprintf('%d', 1-p/255);
end
function printStruct(name, bitmapName)
    fprintf('static const bitmap_t %s = {FONT_WIDTH, FONT_HEIGHT, FONT_COLOR, %s};\n', name, bitmapName);
end
function printBitmapArray(name, bitmapName)
    fprintf('static const int* characters[26*2+10] bitmap_9 = {');
    fprintf('};');
end