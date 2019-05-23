A simple firmware for my hacked up and hand-wired TEK227 keyboard. This keyboard
appeared at time of purchase to be a reasonably priced mechanical keyboard with
some desireable ergonomic features. Unfortunately, after a few months of using
it the chatter problems started to become unbearable. Many similar stories can
be found online, such as [this one](http://www.cataclysmicmutation.com/2014/03/28/truly-unresponsive/).

Years later I am about to write an MA thesis and would prefer not to do it on my
laptop's built in keyboard, but I can't afford to buy a nicer one. Fortunately,
diodes, solder, and wire are cheap, and I had a Teensy LC already lying around.

The main issue is debouncing the keys. This is still a work in progress.
