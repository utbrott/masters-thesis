%% Spectrogram zoom function
function spectrogramzoom(input_file,output_file,opts)
    % Function args
    arguments
        input_file (1,1) string
        output_file (1,1) string
        opts.zoom_range (2,1) double
        opts.window_size (1,1) double = 1024
        opts.sample_rate (1,1) double = 1e6
        opts.n_overlap (1,1) double = 512
        opts.n_fft (1,1) double = 1024
    end

    % Main function
    data = loadfile(input_file);
    window = hamming(opts.window_size);
    figure, colormap('winter');

    subplot(2,1,1); % Full
    spectrogram(data,window,opts.n_overlap,opts.n_fft,...
        opts.sample_rate,'yaxis');
    ylim([-inf 220]);
    title('Full capture');
    xline([opts.zoom_range(1) opts.zoom_range(2)],'r--','LineWidth',1.2);
    xticks(0:1:(length(data)/opts.sample_rate));
    set(gca,'XMinorTick','on');
    hold on;

    subplot(2,1,2); % Zoomed in
    zoom_range = [opts.zoom_range(1)*opts.sample_rate,...
        opts.zoom_range(2)*opts.sample_rate];
    data = data(zoom_range(1):zoom_range(2),:);
    spectrogram(data,window,opts.n_overlap,opts.n_fft,...
        opts.sample_rate,'yaxis');
    ylim([-inf 220]);
    title('Zoomed in');
    hold off;

    % Export result
    exportfigure(gcf,output_file);
end

%% Local functions
function data = loadfile(filename) % Loads data
    f_id = fopen(filename,'rb');
    raw_data = fread(f_id,'float32');
    data = raw_data(1:2:end) + 1j*raw_data(2:2:end);
end

function exportfigure(fig,output_file) % Scale, export figure
    target_width = 1920;
    target_height = target_width/2;
    set(fig,'Position',[200 200 target_width target_height]);
    movegui(fig,'center');
    saveas(fig,output_file,'png');
    close;
end