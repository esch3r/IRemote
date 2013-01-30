m3=4;
m3nut1=5.8;
m3nut2=6.2;
m3nutHeight=3;
m3head=6.5;
m3headHeight=3;
m3length=18;
sideWall=2;
topWall=2;
bottomWall=2;
boardWidth=80;
boardLength=66;
boardThickness=1.6;
boardSideMargin=1;
boardBottomMargin=2;
innerHeight=22;
inset=0.5;

cubeRadius=5;	// radius for the box



// Sidebox
sideBoxBottomWidth=12;
sideBoxBottomLength=12;
sideBoxBottomHeight=boardBottomMargin;

sideBoxTopWidth=6;//m3nut2;
sideBoxTopLength=6;//m3nut1;
sideBoxTopHeight=innerHeight/2-sideBoxBottomHeight;

// Precalculation
finalSideWidth=boardWidth+(sideWall+boardSideMargin)*2;
finalSideHeight=boardHeight+(sideWall+boardSideMargin)*2;

finalWidth=boardWidth+boardSideMargin*2+sideWall*2;
finalLength=boardLength+boardSideMargin*2+sideWall*2+2*sideBoxTopLength;
finalHeight=innerHeight/2+bottomWall;

finalUpperHeight=innerHeight/2+topWall;
finalLowerHeight=finalUpperHeight;


// Buttons
hardwareButtonHeight=9; //in mm from the bottom of the board to the top
buttonTopSize=9;
buttonBottomSize=11;
buttonTopHeight=finalUpperHeight*0.75;
buttonBottomHeight=finalUpperHeight/2+finalLowerHeight-bottomWall-boardBottomMargin-hardwareButtonHeight;	// 8 is the height of a hardware button

buttonRadius=1.5;
button1PosX=17;
button1PosY=13;
button2PosX=30;
button2PosY=13;
button3PosX=43;
button3PosY=13;

//Leds
ledRadius=2;
ledHull=1;
ledHeight=2;

led1PosX=52;
led1PosY=12;
led2PosX=60;
led2PosY=12;
led3PosX=68;
led3PosY=12;



renderButtons=false;
renderTop=true;
renderBottom=true;

rotation=0;		//set to 180 for top export

$fn=16;

//translate([boardSideMargin+sideWall,boardSideMargin+sideWall,bottomWall+boardBottomMargin]) color("green") cube([boardWidth,boardHeight,boardThickness]);

rotate([rotation,0,0])
if (renderTop || renderBottom)
{
difference() {
union() {
	if (renderBottom == true)
	{
	union() {
		difference() {
			roundedCube([finalWidth, finalLength, finalHeight], cubeRadius);
			translate([sideWall, sideWall, bottomWall]) cube([finalWidth-sideWall*2, finalLength-sideWall*2, finalHeight]);
		}
		translate([sideWall,sideWall,bottomWall]) sideBox();		
		translate([finalWidth-sideWall,sideWall,bottomWall]) mirror([1,0,0]) sideBox();
		translate([finalWidth-sideWall,finalLength-sideWall,bottomWall]) rotate([0,0,180]) sideBox();
		translate([sideWall,finalLength-sideWall,bottomWall]) mirror([0,1,0]) sideBox();
	}
	}

	if (renderTop == true)
	{
	color("red") union() {
		// screw holder
		translate([sideWall,sideWall,bottomWall+boardBottomMargin+boardThickness]) sideBoxUpper();
		translate([finalWidth-sideWall,,sideWall,bottomWall+boardBottomMargin+boardThickness]) mirror([1,0,0]) sideBoxUpper();
		translate([finalWidth-sideWall,finalLength-sideWall,bottomWall+boardBottomMargin+boardThickness]) rotate([0,0,180]) sideBoxUpper();
		translate([sideWall,finalLength-sideWall,bottomWall+boardBottomMargin+boardThickness]) mirror([0,1,0]) sideBoxUpper();
		// box
		translate([0,finalLength,finalHeight+finalUpperHeight]) rotate([180,0,0]) difference() {
			roundedCube([finalWidth, finalLength, finalUpperHeight], cubeRadius);
			translate([sideWall, sideWall, topWall]) cube([finalWidth-sideWall*2, finalLength-sideWall*2, finalUpperHeight]);
		}

		// button hulls
		translate([button1PosX,button1PosY,finalLowerHeight+finalUpperHeight-topWall+0.5]) color("red") buttonHull(buttonTopSize+1, buttonTopHeight-topWall-0.5, 3, buttonRadius);
		translate([button2PosX,button2PosY,finalLowerHeight+finalUpperHeight-topWall+0.5]) color("red") buttonHull(buttonTopSize+1, buttonTopHeight-topWall-0.5, 3, buttonRadius);
		translate([button3PosX,button3PosY,finalLowerHeight+finalUpperHeight-topWall+0.5]) color("red") buttonHull(buttonTopSize+1, buttonTopHeight-topWall-0.5, 3, buttonRadius);

		// led hulls
		translate([led1PosX,led1PosY,finalLowerHeight+finalUpperHeight-topWall]) ledHull(ledRadius,ledHull,ledHeight);
		translate([led2PosX,led2PosY,finalLowerHeight+finalUpperHeight-topWall]) ledHull(ledRadius,ledHull,ledHeight);
		translate([led3PosX,led3PosY,finalLowerHeight+finalUpperHeight-topWall]) ledHull(ledRadius,ledHull,ledHeight);
	}
	}
}
	// the screw holes
	translate([sideWall+m3nut2/2,sideWall+m3nut1/2,0]) m3screw();
	translate([finalWidth-sideWall-m3nut2/2,sideWall+m3nut1/2,0]) m3screw();
	translate([sideWall+m3nut2/2,finalLength-sideWall-m3nut1/2,0]) m3screw();
	translate([finalWidth-sideWall-m3nut2/2,finalLength-sideWall-m3nut1/2,0]) m3screw();

	//Button holes
	translate([button1PosX,button1PosY,finalLowerHeight+finalUpperHeight+1]) button(buttonTopSize+1, 1, buttonTopHeight, 1, buttonRadius);
	translate([button2PosX,button2PosY,finalLowerHeight+finalUpperHeight+1]) button(buttonTopSize+1, 1, buttonTopHeight, 1, buttonRadius);
	translate([button3PosX,button3PosY,finalLowerHeight+finalUpperHeight+1]) button(buttonTopSize+1, 1, buttonTopHeight, 1, buttonRadius);
	
	//LED holes
	translate([led1PosX,led1PosY,finalUpperHeight+topWall+5]) cylinder(r=ledRadius,h=10);
	translate([led2PosX,led2PosY,finalUpperHeight+topWall+5]) cylinder(r=ledRadius,h=10);
	translate([led3PosX,led3PosY,finalUpperHeight+topWall+5]) cylinder(r=ledRadius,h=10);

	// Output holes
	translate([-5,52,bottomWall+boardBottomMargin+7]) rotate([0,90,0]) cylinder(r=3.5,h=10);	// IR LED 1
	translate([finalWidth-5,46,bottomWall+boardBottomMargin+7]) rotate([0,90,0]) cylinder(r=3.5,h=10);	// IR LED 2
	translate([finalWidth-5,56,bottomWall+boardBottomMargin+8]) rotate([0,90,0]) cylinder(r=4,h=10);	// TSOP
	translate([-5,22,bottomWall+boardBottomMargin+12+5]) rotate([0,90,0]) roundedCube([5,22,10],1.5);	// USB

}
}

//Buttons
if (renderButtons==true)
{
	translate([button1PosX,button1PosY,finalLowerHeight+finalUpperHeight]) color("green") button(buttonTopSize, buttonBottomSize, buttonTopHeight, buttonBottomHeight, buttonRadius);
	translate([button2PosX,button2PosY,finalLowerHeight+finalUpperHeight]) color("green") button(buttonTopSize, buttonBottomSize, buttonTopHeight, buttonBottomHeight, buttonRadius);
	translate([button3PosX,button3PosY,finalLowerHeight+finalUpperHeight]) color("green") button(buttonTopSize, buttonBottomSize, buttonTopHeight, buttonBottomHeight, buttonRadius);
}


module m3screw() {
	union() {
		translate([0,0,m3headHeight-0.1]) cylinder(r=m3/2,h=m3length+0.1);
		translate([0,0,m3headHeight-0.1]) cylinder(r=m3head/2, r2=m3/2, h=m3headHeight/2+0.1);	// for better printing results
		translate([0,0,-0.1]) cylinder(r=m3head/2,h=m3headHeight+0.1);
	}
}

module sideBoxBottom() {
	cube([sideBoxBottomWidth,sideBoxBottomLength,sideBoxBottomHeight]);
}

module sideBoxTop() {
	cube([sideBoxTopWidth,sideBoxTopLength,sideBoxTopHeight]);
}

sideBoxUpperBottomWidth=sideBoxTopWidth+sideWall+inset;
sideBoxUpperBottomLength=sideBoxTopLength+sideWall+inset;
sideBoxUpperBottomHeight=sideBoxTopHeight-boardThickness;

module sideBoxUpperBottom() {
	difference() {
		cube([sideBoxUpperBottomWidth,sideBoxUpperBottomLength,sideBoxUpperBottomHeight]);
		cube([sideBoxTopWidth+inset,sideBoxTopLength+inset,sideBoxTopHeight]);
		cube([sideBoxUpperBottomWidth,inset,sideBoxUpperTopHeight]);
		cube([inset,sideBoxUpperBottomLength,sideBoxUpperTopHeight]);
	}
}

sideBoxUpperTopWidth=sideBoxUpperBottomWidth;
sideBoxUpperTopLength=sideBoxUpperBottomLength;
sideBoxUpperTopHeight=innerHeight/2;

module sideBoxUpperTop() {
	difference () {
		cube([sideBoxUpperTopWidth,sideBoxUpperTopLength,sideBoxUpperTopHeight]);	
		translate([0,0,sideBoxUpperTopHeight/2]) cube([20,m3nut1,m3nutHeight]);
	}
}

module sideBoxUpper () {
	union() {
		sideBoxUpperBottom();
		translate([0,0,sideBoxUpperBottomHeight]) sideBoxUpperTop();
	}
}

module sideBox() {
	union() {
		translate([0,0,sideBoxBottomHeight]) sideBoxTop();
		sideBoxBottom();
	}
}

module button(topSize, bottomSize, topHeight, bottomHeight, radius)
{
	translate([-topSize/2,-topSize/2,-topHeight])
	union() {
		roundedCube([topSize,topSize,topHeight], radius);
		translate([-(bottomSize-topSize)/2,-(bottomSize-topSize)/2,-bottomHeight]) roundedCube([bottomSize,bottomSize,bottomHeight], radius);
	}
}

module buttonHull(topSize, topHeight, thickness, radius)
{
	translate([-topSize/2,-topSize/2,-topHeight])
	difference() {
		translate([-thickness/2,-thickness/2,0]) roundedCube([topSize+thickness,topSize+thickness,topHeight],radius);	
		translate([0,0,-1]) roundedCube([topSize,topSize,topHeight+2],radius);
	}	
}

module ledHull(radius,thickness, height)
{
	translate([0,0,-height])
	difference() {
		cylinder(r=radius+thickness,h=height);
		translate([0,0,-1]) cylinder(r=radius,h=height+2);
	}
}

module roundedCube(size, radius)
{
x = size[0];
y = size[1];
z = size[2];

translate([x/2,y/2,0])
linear_extrude(height=z)
hull()
	{
	// place 4 circles in the corners, with the given radius
	translate([(-x/2)+(radius), (-y/2)+(radius), 0])
	circle(r=radius);

	translate([(x/2)-(radius), (-y/2)+(radius), 0])
	circle(r=radius);

	translate([(-x/2)+(radius), (y/2)-(radius), 0])
	circle(r=radius);

	translate([(x/2)-(radius), (y/2)-(radius), 0])
	circle(r=radius);
	}
}