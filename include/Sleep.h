//
// Created by adamhoof on 02.03.21.
//

#ifndef UNTITLED1_SLEEP_H
#define UNTITLED1_SLEEP_H

#endif //UNTITLED1_SLEEP_H

void goodNight()
{
    delay(100);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
}
