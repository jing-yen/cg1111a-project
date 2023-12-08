# cg1111a-project
Our group's mBot code for CG1111A lab assignment.

For our CG1111A project, our mBot had to traverse a maze. But it is not so simple. At every junction/turn (known as a Waypoint Challenge), the mBot had to sense a black line and stop, then take colour readings of the ground (using our homegrown colour sensor) to determine which path to go next.

Through this project, I was able to hone my programming and improve my hardware experience, and to work with my team members on an engineering project.

Aside from constructing our own colour sensor circuit using LEDs and an LDR, we also constructed an IR proximity sensor to help with the provided ultrasonic sensor. Calibrating the two homegrown sensors was not easy at all. 

The IR proximity sensor works by detecting the intensity of IR light reflected by the wall. The higher the intensity of light, the lower the voltage output of IR detector. Not only that, our algorithm has to account for ambient light, taking alternate readings with the IR emitter on and off.

![Picture1](https://github.com/jing-yen/cg1111a-project/assets/45094032/a8552ce0-93ed-4658-b2ca-20d11a559a38)

Making the RGB colour sensor work was a more complicated affair. We had to get the reflected intensity readings for red, green and blue colours separately, to detect the amount of coloured light reflected by the surface (hence obtain the colour). That required calibrating the sensor with values for absolute darkness and brightness (black and white). Then, we calculated the Euclidean distance from the 6 known colours, to determine the closest colour, and hence determine our next move.

![Picture6](https://github.com/jing-yen/cg1111a-project/assets/45094032/d400e67b-551f-4176-80fb-02f3799f3382)
![Picture7](https://github.com/jing-yen/cg1111a-project/assets/45094032/0f1943a8-1a32-4439-8f70-8e1016cbef02)
![Picture8](https://github.com/jing-yen/cg1111a-project/assets/45094032/ee71df2f-62d7-46cd-9c17-d2a9a9c2d6ad)
![Picture9](https://github.com/jing-yen/cg1111a-project/assets/45094032/9c3f1b5a-0682-4ff3-b881-d97f9010f5d0)


Overall, I would say the CG1111A project is more challenging than the EG1311 project.

![photo1700099098 (2)](https://github.com/jing-yen/cg1111a-project/assets/45094032/cf8c0e8a-dc18-4363-952e-adcbf279425a)
Pic 1: Our mBot!

![photo1700101175 (3)](https://github.com/jing-yen/cg1111a-project/assets/45094032/6d7f1b90-91ab-4b2f-9c32-a0260653f9df)
Pic 2: Homegrown colour sensor

![photo1700101175 (2)](https://github.com/jing-yen/cg1111a-project/assets/45094032/9c43cd15-ac33-4d40-8849-33a30fe9a6fb)
Pic 3: Homegrown IR proximity sensor
