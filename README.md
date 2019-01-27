# HakimElgabi_ZumoAssignment


Task 1

I used the Processing example ‘SimpleWrite’ and the Arduino example ‘PhysicalPixel’ to understand how to send bytes to the Zumo and for the Zumo to interpret the data it received. Used previous lab sessions to understand how to make the Zumo move forwards and backwards and turn left or right.

Task 2

I used the Arduino examples ‘BorderDetect’ and ‘LineFollower’ to figure how to detect borders and inverted it so that it detected black lines. I put the moveForward function from task 1 into a loop so that it would move forward constantly without user input. If one of the sensors hits a corridor border, it will use the code from task 1 to reverse and then turn depending on which side of the Zumo hit the border.


Task 3

Initially I took the code task 2 but instead of detecting a border with the furthest most sensors on the both sides of the Zumo, I used the sensors in between them to detect borders in front of the Zumo instead. However, I realised that this wasn’t always accurate, so I change the criteria so that it included both the far end sensors detecting them at the same time as well. To understand how to get the UI to receive and display messages, I used the Processing example ‘SimpleRead’, luckily this example also includes the Arduino code to send messages through the Xbees at the bottom of the file in comments. 


Task 4

I used the code from the Arduino example ‘NewPingExample’ to understand how to use the Ultra Sonic sensor so that it can ping objects and calculate their distant. However, in the example the distance is set to 200cm but the rooms in the scenario are only roughly 18cm long and 24 cm wide so I set the distance to 50cm and the criteria for detecting objects is to between 2cm and 20cm. In order to get the Zumo to stop during automated mode (besides hitting a wall) I had to include a small section of code from task 1 so that it would only receive an ‘S’ in the loop for automated movement. Initially I got the Zumo to spin 90 degrees left and right to try and get the Zumo to detect the object in the room, however I realized that I had to increment the turns so that between turns the Ultra sonic sensor can scan properly. Additional buttons were added to map which side the room was on and a counter was added to track what room the Zumo is at on the current corridor.


Task 5

I realised I had to get the Zumo to start tracking how long the corridors are, in order to do this I put an If statement inside the moveForward function so that it will increment a counter by the Delay being called if the Automated mode is true, I also realised that when the Zumo hits the side of corridors it will reverse so it affects the length being counted so I had to also decrement the corridor Length counter by the Delay being called if it hits a border. Once it hits the end of the corridor, it will add the corridor Length counter to an array and then reset the counter for the next corridor.

I added a button to the UI so that it will retrace the previous corridor and when the Zumo receives this byte it will go forward with a delay using the corridor Length of the previous corridor so it will return to the centre of the T-Junction. However, I noticed the Zumo overcompensated for the length of the corridor, so I reduced the length of forward delay by 300 to account for the Zumos length. 


Task 6

I changed the basic arrays for storing the corridor and rooms into class arrays so that I could easily store the different variables needed for the corridors and rooms to optimize the return route. For example, the corridors required the Length, corridor turn type and all the rooms in that specific corridor. The rooms need to store which side of the corridor it is on, whether someone is in the room and the position in the corridor. Since the size of arrays are hardcoded, I needed to use a variable to set whether the instance in the array is being used so that I can determine the ‘true’ size of the array. I added buttons to map the specific corridor turns and tell the Zumo that it’s a T-Junction.

One major problem with the automated return is that I decided to use hard coded ninety degree turns in order to navigate the track. However, the Zumo is extremely inconsistent with its turning speed depending on terrain or battery level and I believe my Zumo generally turned left faster than it did right. In order to compensate for this, I had to add functionality so that I could change the specific turn duration for each wheel of the Zumo at run time by adding an extra window in the UI that will lower or raise the turn duration for each turn direction.

To optimize the return route, the Zumo returns to the T-Junction, checking any rooms with objects already found in them, once it reaches the T-Junction it will ignore the other side depending on whether anyone was found on that side. If someone is found on that it will search that side and once it hits the last room with an object found within it, it will turn around to return to the T-Junction. It will then commence searching the rest of track and check any rooms with objects previously founds in them and ignoring those that don’t.


# Libraries Used

ZumoMotors https://github.com/pololu/zumo-shield 

Used to control motors and to manipulate their speeds for moving forwards, backwards or turning

QTRSensors https://github.com/pololu/zumo-shield 

Used for detecting the borders of the corridors and detecting walls (Black Borders)

ZumoReflectanceSensorArray https://github.com/pololu/zumo-shield 

Also used for detecting borders and for calibration.

NewPing https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home 

Used for ultrasonic sensor to detect objects

G4P http://www.lagers.org.uk/g4p/ -

Helpful tool for building a UI quickly and efficiently in processing


# Summary

I believe I have competently completed all tasks and works reliably most of the time. During the automated movement, return to T-Junction and Return to Home segments the Zumo can detect and stay within the borders.

https://drive.google.com/open?id=1VQcvmv8twfLx4G-Rf3nX0g00jRStM6j4
