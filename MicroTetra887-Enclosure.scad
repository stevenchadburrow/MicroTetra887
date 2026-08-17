// MicroTetra887

module radiusedblock(xlen,ylen,zlen,radius) {
	hull() {
		translate([radius,radius,radius]) sphere(r=radius);
		translate([xlen + radius , radius , radius]) sphere(r=radius);
		translate([radius , ylen + radius , radius]) sphere(r=radius);    
		translate([xlen + radius , ylen + radius , radius]) sphere(r=radius);
		translate([radius , radius , zlen + radius]) sphere(r=radius);
		translate([xlen + radius , radius , zlen + radius]) sphere(r=radius);
		translate([radius,ylen + radius,zlen + radius]) sphere(r=radius);
		translate([xlen + radius,ylen + radius,zlen + radius]) sphere(r=radius);
	}
}

$fn = 20; // higher detail to curves

// mode -> 0 = console, mode -> 1 = controller1, mode -> 2 = controller2
mode = 0;

// half -> 0 = both, half -> 1 = top, half -> 2 = bottom, half -> 3 = buttons
half = 0;

// up, down, left right, A, B
bpx = [ 87.5, 87.5, 95.25, 79.75, 4.75, 15.75 ];
bpy = [ 80.0, 64.5, 72.25, 72.25, 72.25, 72.25 ];

scale([1,-1,1])
{
	difference()
	{
		// outside
		translate([-5,-5,-5])
		{
			radiusedblock(100,100,20,5);
		}
		
		// inside
		translate([-1,-1,0])
		{
			cube([102,102,20]);
		}
			
        if (mode == 0)
        {
            // vga
            translate([11.0,-10,5])
            {
                cube([33.0,20,14]);
            }
            
            // reset
            translate([50.0,-10,9])
            {
                rotate([-90,0,0])
                {
                    cylinder(20,3,3);
                }
            }
                
            // usb
            translate([58.0,-10,5])
            {
                cube([14.0,20,12]);
            }
            
            // switch
            translate([77.0,-10,7])
            {
                cube([11.0,20,7]);
            }
        }
        
        if (mode == 0 || mode == 1)
        {
            // joy1
            translate([13.5,90,5])
            {
                cube([33.0,20,14]);
            }
        }
        
        if (mode == 0 || mode == 2)
        {
            // joy2
            translate([53.5,90,5])
            {
                cube([33.0,20,14]);
            }
        }
        
        if (mode == 1 || mode == 2)
        {
            // buttons
            for (i = [0:5])
            {
                translate([bpx[i], bpy[i], 10])
                {
                    cylinder(20,4,4);
                }
            }
        }
		
		// screw holes
		translate([4.0,4.0,5])
		{
			rotate([180,0,0])
			{
				cylinder(20,2.5,2.5);
			}
		}
		translate([96.0,4.0,5])
		{
			rotate([180,0,0])
			{
				cylinder(20,2.5,2.5);
			}
		}
		translate([4.0,96.0,5])
		{
			rotate([180,0,0])
			{
				cylinder(20,2.5,2.5);
			}
		}
		translate([96.0,96.0,5])
		{
			rotate([180,0,0])
			{
				cylinder(20,2.5,2.5);
			}
		}
		
        if (half == 1 || half == 3)
        {
            // comment out for top only
            translate([-10,-10,-10])
            {
                cube([120,120,10+5]);
            }
        }
		
        if (half == 2 || half == 3)
        {
            // comment out for bottom only
            translate([-10,-10,5])
            {
                cube([120,120,30]);
            }
        }
	}
	
	difference()
	{
		union()
		{
			// screw supports
			translate([4,4,0])
			{
				difference()
				{
					cylinder(20,4.5,4.5);
					cylinder(20,1.0,1.0);
				}
			}
			translate([96,4,0])
			{
				difference()
				{
					cylinder(20,4.5,4.5);
					cylinder(20,1.0,1.0);
				}
			}
			translate([4,96,0])
			{
				difference()
				{
					cylinder(20,4.5,4.5);
					cylinder(20,1.0,1.0);
				}
			}
			translate([96,96,0])
			{
				difference()
				{
					cylinder(20,4.5,4.5);
					cylinder(20,1.0,1.0);
				}
			}
			
			// support supports
			translate([1,1,0])
			{
				cylinder(20,3,3);
			}
			translate([99,1,0])
			{
				cylinder(20,3,3);
			}
			translate([1,99,0])
			{
				cylinder(20,3,3);
			}
			translate([99,99,0])
			{
				cylinder(20,3,3);
			}
			
            if (mode == 1 || mode == 2)
            {
                // button supports
                for (i = [0:5])
                {
                    translate([bpx[i], bpy[i], 15])
                    {
                        difference()
                        {
                            cylinder(5,6,6);
                            cylinder(5,4,4);
                        }
                    }
                }
            }
		}
		
		// pcb
		translate([-10,-10,5-1.6])
		{
			cube([120,120,1.6]);
		}
		
        if (half == 1 || half == 3)
        {
            // comment out for top only
            translate([-10,-10,-10])
            {
                cube([120,120,10+5]);
            }
        }
		
        if (half == 2 || half == 3)
        {
            // comment out for bottom only
            translate([-10,-10,5])
            {
                cube([120,120,30]);
            }
        }
	}
}

module button()
{
	difference()
	{
		union()
		{
			cylinder(15, 3.5, 3.5);
			cylinder(2, 5, 5);
			intersection()
			{
				sphere(17);
				cylinder(100, 3.5, 3.5);
			}
		}
		translate([-1.35, -1.35, 0])
		{
			cube([2.7, 2.7, 3.0]);
		}
	}
}

if (half == 3)
{
    button();
}












