x=0:2*pi/256:2*pi-2*pi/256;
y=round(128*(sin(x)+1));
plot(y);

file=fopen('sin_tab.txt','wt');

fprintf(file,'int sin_tab[256]={');
fprintf(file,'%d, ',y(1:255));
fprintf(file,'%d',y(256));
fprintf(file,'};');

fclose(file);