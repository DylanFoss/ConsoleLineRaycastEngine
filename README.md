# ConsoleLineRaycastEngine
An attempt of creating a raycaster, using the windows console and ascii characters to display the world. Using a DDA algorithim for it's raycasting methods, a 2D array of 'Walls' can be found from the players postion, and a room can be drawn just using the distances from these walls.
 
![image](https://user-images.githubusercontent.com/57671477/190220927-e3f8dc7c-e032-4367-a97f-08e7c681167f.png)

(moire effect!)

## Controls

A and D to strafe left or right.

W and S to walk forwrd or back.

Q and E to turne left or right.

SPACE to turn 'fisheye' correction on or off

## Demonstration:

https://youtu.be/xV7kghiWD5s

## Known issues:

Since this uses the windows console for visuals, previous settings may make the image disorted. Below are the correct settings that can be entered by right clicking on the console and selecting 'properties'

![image](https://user-images.githubusercontent.com/57671477/190221651-c679b2e6-c156-42a6-9106-9ff51859bbd9.png)
![image](https://user-images.githubusercontent.com/57671477/190221589-b3323ab4-a399-4c67-ba6c-1412759b3250.png)

### Update 31/01/23

Opening the .exe with the newer terminal (Windows Host) does not bode well. This can be fixed by switiching back to the old Windows terminal by default, or more simply running the .exe as an administrator.

## Special Thanks:
To these resources!

[Raycasting - Lode Vandevenne](https://lodev.org/cgtutor/raycasting.html)

[Super Fast Ray Casting in Tiled Worlds using DDA - One Lone Coder](https://www.youtube.com/watch?v=NbSee-XM7WA)
