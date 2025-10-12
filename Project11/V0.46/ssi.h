#ifndef __SSI_H__
#define __SSI_H__

const char *ssitags[] = {"text1", "text2", "text3", "text4", "text5", "text6", "text7", "text8", "text9", "text10", "text11", "text12", "text13", "text14", "text15", "text16",
	                     "text17", "text18", "text19", "text20", "text21", "text22", "text23", "text24", "text25", "text26", "text27", "text28", "text29", "text30", "text31", "text32",
	                     "text33", "text34", "text35", "text36", "text37", "text38", "text39", "text40", "text41", "text42", "text43", "text44", "text45", "text46", "text47", "text48",
	                     "text49", "text50", "text51", "text52", "text53", "text54", "text55", "text56", "text57", "text58", "text59", "text60"};

u16_t mySSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{    
    //uint32_t absoluteTimeMinutes = (to_ms_since_boot(get_absolute_time())/(1000*60));
    //uint32_t absoluteTimeHours = (to_ms_since_boot(get_absolute_time())/(1000*3600));
    
    switch(iIndex){
        
        case 0:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[0]);
            break;
        case 1:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[1]);
            break;
        case 2:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[2]);
            break;
        case 3:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[3]);
            break;
        case 4:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[4]);
            break;
        case 5:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[5]);
            break;
        case 6:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[6]);
            break;
        case 7:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[7]);
            break;
        case 8:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[8]);
            break;
        case 9:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[9]);
            break;
        case 10:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[10]);
            break;
        case 11:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[11]);
            break;
        case 12:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[12]);
            break;
        case 13:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[13]);
            break;
        case 14:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow1[14]);
            break;
        
            
        case 15:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[0]);
            break;
        case 16:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[1]);
            break;
        case 17:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[2]);
            break;
        case 18:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[3]);
            break;
        case 19:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[4]);
            break;
        case 20:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[5]);
            break;
        case 21:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[6]);
            break;
        case 22:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[7]);
            break;
        case 23:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[8]);
            break;
        case 24:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[9]);
            break;
        case 25:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[10]);
            break;
        case 26:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[11]);
            break;
        case 27:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[12]);
            break;
        case 28:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[13]);
            break;
        case 29:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow2[14]);
            break;
            
            
        case 30:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[0]);
            break;
        case 31:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[1]);
            break;
        case 32:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[2]);
            break;
        case 33:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[3]);
            break;
        case 34:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[4]);
            break;
        case 35:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[5]);
            break;
        case 36:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[6]);
            break;
        case 37:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[7]);
            break;
        case 38:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[8]);
            break;
        case 39:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[9]);
            break;
        case 40:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[10]);
            break;
        case 41:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[11]);
            break;
        case 42:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[12]);
            break;
        case 43:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[13]);
            break;
        case 44:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow3[14]);
            break;
        
        case 45:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[0]);
            break;
        case 46:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[1]);
            break;
        case 47:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[2]);
            break;
        case 48:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[3]);
            break;
        case 49:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[4]);
            break;
        case 50:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[5]);
            break;
        case 51:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[6]);
            break;
        case 52:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[7]);
            break;
        case 53:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[8]);
            break;
        case 54:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[9]);
            break;
        case 55:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[10]);
            break;
        case 56:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[11]);
            break;
        case 57:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[12]);
            break;
        case 58:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[13]);
            break;
        case 59:
            snprintf(pcInsert, iInsertLen, "%d", decimalValueTemperatureRow4[14]);
            break;
           
    }
}

#endif
