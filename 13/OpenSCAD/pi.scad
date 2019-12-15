// Raspberry Pi Zero size [mm]:
l = 65;
w = 30;
h = 7;

d = 2; // wall thickness
r = 6; // hole radius
difference() {
  cube([l + 2 * d, w + 2 * d, h + d]);
  translate([d, d, d]) {
      cube([l, w, h]);
  }
  translate([(l / 2) + d, (w / 2) + d], 0) {
      cylinder(d, r, r, false);
  }
}