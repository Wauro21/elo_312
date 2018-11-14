void display_clock(void)
{
   int time = 0;
   while(1)
      {
         int min = time/60;
         int sec = time%60;
         display_clear();
         printf("     %02d:%02d", min, sec);
         putchar('\n');
         udelay(1000000);
         time += 1;
      }
}
