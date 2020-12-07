void compute_intensity(rect r,point p){
  point shift;    
  if(p.x==1024 && p.y==1024) {shift.x=0;shift.y=0;}
  else{
      shift.x=1024-p.x;
      shift.y=1024-p.y;
  }
  p.x=p.x+shift.x;
  p.y=p.y+shift.y;
  int x0 = (r.x + shift.x / sc - ox);
  int y0 = (r.y + shift.y / sc - oy);
  int x1 = x0 + shift.x + r.w / sc;
  int y1 = y0 + shift.y + r.h / sc;
  char* str1;
  char* str2;
  char* str3;
  char* str4;
  str1=make_key(x0,y0,0);
  str2=make_key(x0,y1,1);
  str3=make_key(x1,y0,1);
  str4=make_key(x1,y1,1);
  printf("%s %s %s %s\n",str1,str2,str3,str4);
  kiss_fft_cpx noise;
  kiss_fft_cpx a;
  kiss_fft_cpx b;
  kiss_fft_cpx c;
  kiss_fft_cpx d;
  a=findkey(str1);
  b=findkey(str2);
  c=findkey(str3);
  d=findkey(str4);

  double total=0,real=0,imag=0;

  noise.r=-0.0027;
  noise.i=-0.001869;
  int noise1=0;
  //////////////////////////////////////
  if(type){ //psm
      real=(d.r-b.r-c.r+a.r); //assumed formula
      imag=(d.i-b.i-c.i+a.i);
      if(noise1){real=real+noise.r; imag=imag+noise.i;}
      else {real=real; imag=imag;}
      total=86.943428*(real*real+imag*imag);
      printf("PSM\n");
  }
  else{ //binary
      real=(d.r-b.r-c.r+a.r); //assumed formula
      imag=(d.i-b.i-c.i+a.i);
      total=86.943428*(real*real+imag*imag);
      printf("BINARY\n");
  }
  printf("\nthe intensity is %lf %lf %lf\n",real,imag,total);
  fprintf(stderr,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",a.r,a.i,b.r,b.i,c.r,c.i,d.r,d.i,real,imag,total);
    fprintf(fp,"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",a.r,a.i,b.r,b.i,c.r,c.i,d.r,d.i,real,imag,total);
    fclose(fp);
 ///////////////////////////////////////////// 
}