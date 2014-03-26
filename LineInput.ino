/*-----------------------------------------------------------------------------
 
 Simple Line Input routine.
 By Allen C. Huffman (alsplace@pobox.com)
 www.subethasoftware.com
 
 HACKED to look for EzKey data.
 -----------------------------------------------------------------------------*/

#define CR           13
#define BEL          7
#define BS           8
#define CAN          24

byte lineInput(char *buffer, size_t bufsize)
{
  char    ch;
  byte    len = 0;
  boolean done;

// TODO: spinning cursor!

  done = false;
  while(!done)
  {
    // Handle any input from the EZKey
    if (EZKey.available()>0)
    {
      Serial.print(F("EZ-Key: "));
      while(EZKey.available()>0)
      {
        char ch;
        Serial.write(EZKey.read());
        //if (ch==CR) break;
      }
    }

    //ledBlink();
    if (Serial.available()>0)
    {  
      ch = Serial.read();
      switch(ch)
      {
      case CR:
        Serial.println();
        buffer[len] = '\0';
        done = true;
        break;

      case CAN:
        Serial.println(F("[CAN]"));
        len = 0;
        break;

      case BS:
        if (len>0)
        {
          Serial.write(BS);
          Serial.print(F(" "));
          Serial.write(BS);
          len--;
        }
        break;

      default:
        // If there is room, store any printable characters in the line.
        if (len<bufsize)
        {
          if ((ch>31) && (ch<127)) // isprint(ch) does not work.
          {
            // UPPERCASE it...
            ch = toupper(ch);
            Serial.write(ch);
            buffer[len] = toupper(ch);
            len++;
          }
        }
        else
        {
          Serial.write(BEL); // Overflow. Ring 'dat bell.
        }
        break;
      } // end of switch(ch)           
    } // end of if (Serial.available()>0)
  } // end of while(!done)

  return len;
} // end of lineInput()

/*---------------------------------------------------------------------------*/
// End of LineInput

