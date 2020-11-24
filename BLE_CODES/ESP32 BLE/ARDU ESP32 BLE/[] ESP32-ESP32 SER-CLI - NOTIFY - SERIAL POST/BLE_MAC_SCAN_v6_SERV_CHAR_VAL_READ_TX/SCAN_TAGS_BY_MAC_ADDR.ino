void SCAN_BLE_TAGS_BY_MAC_ADDR() 
{ Serial.println("\n\nInside BLE_SCAN_TAGS_BY_MAC_ADDR()");
  while (scanResultDevicesFound.getCount() >= 1)
  { for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
    { if(Scanned_BLE_Address == BLE_Tag_Addrs_to_Scan[i]) // && paired[i] == 0) //FOUND NEW DEVICE
      { TagFoundStatus[i]=1;
        Serial.println("\n "+BLE_Tag_Addrs_to_Scan[i]+" FOUND BLE SERVER DEVICE IN RANGE");    
        Serial.println(" ... connecting to Server as Client\n");
        
        if (connectToServer(*Server_BLE_Address))
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
 /*     else
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
      }*/
    }
  }   
}

//      Serial.println("Found BLE Server Device ... TURNING ON LED ON LED_PIN"); digitalWrite (LED_PIN,HIGH);
//      Serial.println("Found BLE Server Device ... TOGGLING LED ON LED_PIN");      toggle_led(10); 

/*
void BLE_SCAN_TAGS()
{ scanResultDevicesFound = pBLEScan->start(scanTime, false); // BLEScanResults 
  //Scan for 'scanTime' seconds to find the Advertising Tag
  
//  Serial.print("\nDevices found: ");  Serial.println(scanResultDevicesFound.getCount());  
//  Serial.println("Scan done!");

//  Serial.println("\n\nScanData : "+ScanData+"\n");  
//  Serial.println("\nKNOWN_TAGS_COUNT : "+String(KNOWN_TAGS_COUNT)+"\n");

  FOUND = true; // false; 
  Status = "1";
  
  for(uint8_t i=0; i<KNOWN_TAGS_COUNT; i++)
  {
    if(ScanData.indexOf(BLE_Tag_Addrs_to_Scan[i]) > 0)
    {  
//      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" FOUND - DEVICE IN RANGE\n");
//       FOUND = true; digitalWrite (INDICATOR_PIN, LOW);
      
    
    }
    else
    {  
//      Serial.println("\n\n "+BLE_Tag_Addrs_to_Scan[i]+" NOT FOUND - OUT OF RANGE!!\n");
       FOUND = false; digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);
       Status = "0"; 
    }
  }
  
  ScanData="";
 
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
    
  if(FOUND) {digitalWrite (INDICATOR_PIN, LOW);  digitalWrite (LED_PIN, LOW); }
  else      {digitalWrite (INDICATOR_PIN, HIGH); digitalWrite (LED_PIN, HIGH);}  
}
*/
