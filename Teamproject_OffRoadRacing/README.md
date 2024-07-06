# How to run
- `Install CSS`
- `Add Clock Library`
- `Connect LineTracer`
- `Build and Run`

# Goal
**Race on a track as fast as you can**
- Challenge 1 - Easy Track
  - 1 Lap
- Challenge 2 - Main Track
  - 2 Lap
  - Noise on The Road
  - Must stop after 2 lap

# Specification  

Road Size
- 19mm (2 sensor, default)
- 15mm ~ 30mm

Noise Type
- broken road
- old road

Starting Line
- 57mm * 30mm

# Easy-Track

## Variables

Variables for Sensor(int) : sensor1, sensorleft, sensorright

Variable for Lab(int): lab

```c
sensor1 = P7->IN & 0x18;
sensorleft = P7->IN & 0x60;
sensorright = P7->IN & 0x06;
```

## Routine

1. sensorleft is on black line and sensorright is not on black line
    
    if (sensorleft && !sensorright)   
    
    → turn left
    
    left_backward();     
    right_forward();    
    move(1000, 950);
  
2. sensorright is on black line and sensorleft is not on black line
    
    else if (sensorright && !sensorleft)
    
      → turn right
      
      left_forward();    
      right_backward();    
      move(1000,950);
    
3. sensor1, sensorleft, sensorright are on black line
    1. lab is zero    
    
        → go straight
      
        left_forward();       
        right_forward();      
        move(1500, 1450);
      
    2. lab is 1
    
        → stop
      
        move(0,0);      
        break;
    
4. only sensor1 is on black line
    
    → go straight and make lab 1
    
    lab =1;
    
    left_forward();     
    right_forward();     
    move(1500, 1450);
    

In order to stop at start line after first lab, suppose that sensors are on strat line when starting test, make lab 1 when first called routine 4. It stops when reaches start line next time.

# Main-Track

## Variables

Variables for Sensor(int) : 

sensorcenter, sensorleftInner, sensorleftOuter, sensorrightInner, sensorrightOuter, sensorleftmost, sensorrightmost

```c
sensorcenter = P7->IN & 0x18;
sensorleftInner = P7->IN & 0x20;
sensorleftOuter = P7->IN & 0x40;
sensorrightInner = P7->IN & 0x04;
sensorrightOuter = P7->IN & 0x02;
sensorleftmost = P7->IN & 0x80;
sensorrightmost = P7->IN & 0x01;
```

Variables for Lab(int) : lab, labplus

Variable for Turn(int) : statusLR, statusAll, statusTurnComplete

## Core Routine

1. sensorcenter is on black line (sensorcenter == 1)
    
    After completely turn, make variables for turn zero
    
    ```c
    if(statusTurnComplete == 1 && (!sensorleftInner ||
    !sensorrightInner) && !sensorleftOuter && 
    !sensorrightOuter && !sensorleftmost && 
    !sensorrightmost){
       statusLR = 0;
       statusTurnComplete = 0;
    }
    ```
    
     1. sensorleftInner and sensorleftOuter are on black line (sensorleftInner == 1 && sensorleftOuter == 1 && sensorrightOuter == 0)
        
        → need turn left (statusLR = 1)
      
     2. sensorrightInner and sensorrightOuter are on black line
      (sensorrightInner == 1 && sensorrightOuter == 1 &&  sensorleftOuter == 0)
      
        → need turn right (statusLR = 2)
      
     3. sensorleftInner is on black line (sensorleftInner == 1 && sensorleftOuter == 0) 
      
        → left curve (move(2300, 500))
      
     4. sensorrightInner is on black line (sensorrightInner == 1 && sensorrightOuter == 0)
      
        → right curve (move(800, 2100))
      
     5. only sensorcenter is on black line (else)
      
        → go straight (move(2300, 2250))
        
2. sensorcenter is on white (sensorcenter == 0)

    a. turn left (statusLR == 1)

    ```c
    left_backward();
    right_forward();
    move(2500, 2450);
    statusTurnComplete = 1;
    ```

    b. turn right (statusLR == 2)

    ```c
    right_backward();
    left_forward();
    move(2400,2350);
    statusTurnComplete = 1;
    ```

## Noise Handling Algorithm

<img src="https://github.com/0214wnstjd/ITE2038/assets/109850168/b1aadfc0-7c7f-42c7-bb4e-b3d2f59f5290" width="80%" height="80%" title="이미지1"></img><br/>
1. When either go straight or turn right is OK, then it should go striaght or turn right?

    The code for turn is implemented when sensorcenter is not on black line, so it goes straight rather than turn right.

2.  When either turn to left or right is OK, then turn left or right?

    In this track, turn left is better for reducing time. So use int variable statusAll to distinguish this special case. 

    Inside Core Routine 1.a, add additional code.

    All of sensor variables are 1, make statusLR and statusAll 1.

    In order to keep statusLR 1, for core routine 1.a and 1.b, add additional condition statusAll == 0.

    Make statusAll 0, when core routine 2.a is called.

    <img src="https://github.com/0214wnstjd/ITE2038/assets/109850168/a47f0d19-4730-4fb9-8f88-14a87b0bd164" width="80%" height="80%" title="이미지1"></img><br/>

3. Catch Start Line

    Inside core routine 1, add the following code.

    ```c
    if (sensorleftInner && sensorleftOuter 
         && sensorrightInner && sensorrightOuter 
         && !sensorleftmost && !sensorrightmost){
        labplus = 1;
        if(lab == 1){
            move(0,0);
            return;
        }
    }
    ```

    Initially lab is 0, after complete the first lab, lab changes to 1 at the first turn right.
    
    Inside core routine 2.b, when labplus == 1, do lab++ and labplus = 0.
    
    Hence, when it reaches startLine twice, it stops and return.


## Record

03:02:53
