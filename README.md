<header>

<!--
  <<< Author notes: Course header >>>
  Include a 1280×640 image, course title in sentence case, and a concise description in emphasis.
  In your repository settings: enable template repository, add your 1280×640 social image, auto delete head branches.
  Add your open source license, GitHub uses MIT license.
-->

# RAVEDM 4X4X4: 

_RAVEDM or Rgb Audio Visualizing Electro Dancable Music 4x4x4_ (I tried my best to fit it into the title.
It is a 4x4x4 Green LED cube as of now(I couldn't make RGB possible as of now due to the high price ofdata transfer LEDS,
and my inability to control 28 (8 for each color and 4 for ground)  pins with PWM. Even MEGA has 12 PWMs as far as I can remember(fact check this), I would require 2 Megas ans one Arduino atleast 
or some other solution. I will explain my project below.

</header>

<!--
  <<< Author notes: Finish >>>
  Review what we learned, ask for feedback, provide next steps.
-->

## Components: 

- 4× 4-bit shift registers to have individual control of each LED seperately
- 64 LEDS(any color) and a lot more that will be sacrificed in the process
- I used Galvanised Iron for the 16 Rods on which the LEDS go,recommendef by a friend,
  there might me better options available online
- A solder and flux woulda absolutely necessary, I made the whole ground base for the 4 with the solder and attatched the r and the b of the rgb LEDS(I thought I had bought the data transfer LEDS,(past me should have seen the code first)
- I used an acrylic base and an acrylic box to store your circuit.
- A breadboard and wires( a circuit mode can also be used to save space and to make it look asthetically cooler )


[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/rdp_NPq-_nU/0.jpg)](https://www.youtube.com/watch?v=rdp_NPq-_nU)

### Process
- Each LED in a height layer is attatched to the other through its ground, thus making the ground for each layer the same.These went to the pins 10-13 on the MEGA  
- And there are 16 rods of galvanised iron, on which 4 leds are attatched each.
- The rods of each row are connected to the shift register data lines (Qa to Qd). therefore Qa to Qd connected to the Arduino MEGA represented each row. These went to pins 2-5.
- The clocks were assigned to pins 6-9 and represented the columns as the 4 rods of the column were conneted to one shift register each.
- I used the 5V pin for the Power and it worked just fine.

### Theory
- We used Pulse Width Modulation using the shift registers to control each LED individually.
- Ok So when a signal(5V) was sent to the clock it became ready to be latched, That means if we sent a signal on pin7 let say, then column 2 becomes activated.
- Then the signal is sent to the Qa-Qd (2-5) pins attatched to each row. If we sent High Signal to pin 2 lets say than (1,2) rod gets activated and all the 4 LEDS should light up, but that doesn't happen because We have the grounds (10-13) all in high state. Not we make the particular pin go low voltage and that particular LED lights up.
- The clock signal needs to be sent fist cause unless the clock is in high state the Qa-Qd signal won't be latched to the particular rod.
- When controlling 2 LEDS simultaneously on 2 different rod can become difficult as if I turn on 2 different rods and I switch off the ground  the LEDS of both the grounds of the same layer will light up. This doesn'y give us complete independence on each LED.The solution to this was PWM which allows us to use Multiplexing as we would send one high signal and then after a very short time(for the exact time see code I am lazy) switch it off and then send the signal for the other LED and then repeat the process for the other LED. Both the LEDS are never turned on together but because the time delay is so short, our eyes fail to perceive the change and we see both of them turned on together. This can be seen in the video at the last as I turn on the firefly animation. One problem I faced with PWM is that if becomes difficult to have say  20+ fireflies as the time difference becomes too slow for our eyes to not perceive ( time  =  t initial * 20).
- One thing regarding the code, it was heavily AI influenced as I had little time before the deadline bacause of the RBG LED fk up I did. So sorry for that.   

<footer>

<!--
  <<< Author notes: Footer >>>
  Add a link to get support, GitHub status page, code of conduct, license link.
-->


</footer>
