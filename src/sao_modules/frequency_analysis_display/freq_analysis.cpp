#ifndef FREQ_ANALYSIS_C
#define FREQ_ANALYSIS_C

#include "freq_analysis.hpp"



void FrequencyAnalysisDisplayModule::draw()
{
  int HighestPeak;
  int Height;

  //draw X axis notches:
  for(byte x = 0; x < 128; x+=10)
  {
    //draw notches every 10 ticks to allow identifying the range of frequencies
    //u8g2.drawVLine(x,62,2);
    activeDisplay->drawVLine(x,62,2);
  }

}

#endif
