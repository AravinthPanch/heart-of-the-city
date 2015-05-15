clear ecg samplingrate corrected filtered1 peaks1 filtered2 peaks2 fresult
load ecgdemodata1;

fresult=fft(ecg);

corrected=real(ifft(fresult));
subplot(1, 2, 1);plot((corrected-min(corrected))/(max(corrected)-min(corrected)));

fresult(1 : round(length(fresult)*5/samplingrate))=0;
fresult(end - round(length(fresult)*5/samplingrate) : end)=0;

corrected=real(ifft(fresult));
subplot(1, 2, 2);plot((corrected-min(corrected))/(max(corrected)-min(corrected)));