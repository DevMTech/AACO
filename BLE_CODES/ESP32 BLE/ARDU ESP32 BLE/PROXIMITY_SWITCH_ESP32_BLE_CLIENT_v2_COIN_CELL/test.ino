/*void BLE_SCAN_TAGS_BY_MAC_ADDR() 
{ while (foundDevices.getCount() >= 1)
  { for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
    { if(Scanned_BLE_Address == BLE_Tag_Addrs_to_Scan[i] && paired[i] == 0) //FOUND NEW DEVICE
      { TagFoundStatus[i]=1;
        Serial.println("\n "+BLE_Tag_Addrs_to_Scan[i]+" FOUND BLE SERVER DEVICE IN RANGE");    
        Serial.println(" ... connecting to Server as Client\n");
        
        if (connectToserver(*Server_BLE_Address))
        { paired[i] = 1;
//          Serial.println("********************LED turned ON************************");  digitalWrite (LED_PIN,HIGH);
          break;
        }
        else
        { paired[i] = 0;
          Serial.println("Pairing failed");
          break;
        }
      }
      else
      { 
        
        //      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" NOT FOUND - OUT OF RANGE!!\n");
        //      Serial.println("DID NOT FIND BLE Server Device ... TURNING OFF LED ON LED_PIN");  digitalWrite (LED_PIN,LOW);
        //      FOUND = 0; Status = "0";
      }
      
      if (Scanned_BLE_Address == BLE_Tag_Addrs_to_Scan[i] && paired[i] == 1) // FOUND PREVIOUSLY PAIRED TAG AGAIN
      { TagFoundStatus[i]=1;
        paired[i] = 0;
        
//        Serial.println("Our device went out of range");
//        Serial.println("********************LED OOOFFFFF************************");    digitalWrite (LED_PIN,LOW);         
        break; //        ESP.restart();
      }
      else
      { Serial.println("We have some other BLe device in range");
        break;
      }
    }
  }   
}*/

//      Serial.println("Found BLE Server Device ... TURNING ON LED ON LED_PIN"); digitalWrite (LED_PIN,HIGH);
//      Serial.println("Found BLE Server Device ... TOGGLING LED ON LED_PIN");      toggle_led(10); 
