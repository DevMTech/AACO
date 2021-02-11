

#define RANDM_STR_ELEMENTS 10+26+26
#define ARRAY_SIZE RANDM_STR_ELEMENTS+2
#define RANDOM_NUM_UPPER_RANGE 50000
#define ArraySize(x) (int)(sizeof(x)/sizeof(x[0]))
#define RANDM_SEED_PIN 34

void createRandomString()
{
  //int searchList[ARRAY_SIZE]; //  ARRAY_SIZE 200
  int index, i = 0, j;
  //int target = 5343; // Part number to find
  //long randomNumber = 0L; // 'L' tells compiler it's a long data type, not an int.
  char randChars[ARRAY_SIZE];
  
  //for(i=0; i< ArraySize(randChars);)
  {
    for(j=48; j<58; j++)
    {
      randChars[i]=(char)j;
      i++;
    }
    for(j=65; j<91; j++)
    {
      randChars[i]=(char)j;
      i++;
    }
    for(j=97; j<123; j++)
    {
      randChars[i]=(char)j;
      i++;
    }
    //break;
    //randChars[i] = '\0';
  }
  randChars[i] = '\0';

  //Serial.println(randChars);
  
  // memset(searchList, 0, sizeof(searchList)); // Clear the array
  //  memset(300, 0, 400); // Go to memory location 300 and set the next 400 bytes of memory to zero.
  // 200 elements at 2 bytes for each element = 400 bytes
  pinMode(RANDM_SEED_PIN, INPUT);
  randomSeed(analogRead(RANDM_SEED_PIN)); // This seeds the random number generator

  for(i=0; i<10; i++)
  {
    int randN = (int) random(1000, RANDOM_NUM_UPPER_RANGE);
    
    randString += String(randChars[randN%(RANDM_STR_ELEMENTS)]);
  }

  //Serial.println(randString);
  

//  //  for(index = 0; index < ARRAY_SIZE; index++)
//  //  for (index = 0; index < (int)(sizeof(searchList) / sizeof(searchList[0])); index++)
//  for (index = 0; index < ArraySize(searchList); index++)
//  { searchList[index] = (int) random(0, ARRAY_ELEMENT_VALUE_RANGE); // ARRAY_ELEMENT_VALUE_RANGE 50000
//  }
//
//  DEBUG_Sprintln("\nPOPULATED ARRAY: ");
//
//  //  for (index = 0; index < ARRAY_SIZE;)
//  for (index = 0; index < ArraySize(searchList);)
//  { if (index % COLUMNS == 0) DEBUG_Sprintln();
//    DEBUG_Sprint("\t\t" + String(searchList[index++]));
//  }
//
//  randomNumber = random(0, ARRAY_SIZE); // Random numbers between 0 and one million
//  DEBUG_Sprintln("\n\nRANDOM INDEX : " + String(randomNumber));
//
//  index = (int) randomNumber;
//  //  searchList[(int)randomNumber] = target; // Our target
//  searchList[index] = target;
//
//  DEBUG_Sprintln("Target value " + String(target) + " inserted at index " + String(index) + "\n");
//
//  index = 0; // Expression1 of well-behaved loop
//
//  while (true) // Expression2 of well-behaved loop
//  { if (searchList[index] == target)
//    { break;
//    }
//    index++; // Expression3 of well-behaved loop
//  }
//  DEBUG_Sprint("Found target at index = "); // Display result
//  DEBUG_Sprintln(index);
//  DEBUG_Sprintln("\n");
 
}
