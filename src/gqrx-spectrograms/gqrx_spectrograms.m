clear; clc; close;

output_folder = './_gqrx/';
signal_number = 3;
window_size = 1024;

if ~isfolder(output_folder)
    mkdir(output_folder);
end

for i = 0:3
    input_file = sprintf('./_raw/gqrx_signal_%i.raw',signal_number);
    output_file = sprintf('%szoom-win%i-sig%i-lvl%i',...
        output_folder,window_size,signal_number,4-i);

    spectrogramzoom(input_file,output_file,...
        'zoom_range',[12-(0.35*i),12.95+(0.35*i)],...
        'window_size',1024,...
        'sample_rate',1e6);
end