provider histogram_equalization {

   probe equalization__start(int size);
   probe computing__done(int size);
   probe normalizing__done(int size);
   probe mapping__done(int size);

   probe after__read(int size);
   probe before__write(int size);




};