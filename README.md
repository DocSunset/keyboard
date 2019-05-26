A simple firmware for my hacked up and hand-wired TEK227 keyboard. This keyboard
appeared at time of purchase to be a reasonably priced mechanical keyboard with
some desireable ergonomic features. Unfortunately, after a few months of using
it the chatter problems started to become unbearable. Many similar stories can
be found online, such as [this one](http://www.cataclysmicmutation.com/2014/03/28/truly-unresponsive/).

Years later I am about to write an MA thesis and would prefer not to do it on my
laptop's built in keyboard, but I can't afford to buy a nicer one. Fortunately,
diodes, solder, and wire are cheap, and I had a Teensy LC already lying around.

Some of the keys on my keyboard are aggresively chatty; even when held firmly,
the measurements produced are often extremely erratic, and I can almost forgive
the original designers for failing to effectively debounce them. For example,
here are a few bit streams representing 32 consecutive button readings which I
observed while trying to figure out how I could possibly debounce the keys in
software. These were all observed while holding down a key firmly, trying to
move as little as possible.

Most of the time with most of the keys, I would see the expected stream of ones
meaning that the key is pressed:

```
1111 1111 1111 1111 1111 1111 1111 1111
```

But some of the keys were less well behaved, giving readings like the following:

```
1110 0010 0011 1010 1111 1000 0110 1100
0010 0111 1101 0000 0110 0111 1110 1000
1000 0000 1110 1111 1101 1111 1110 0001
```
etc.

None of the debounce algorithms I read about seemed likely to be able to make
sense of this kind of chatter. One thing I recognized though was that all of the
keys were very consistent when they were not being pressed. In this case, they
all produced a constant stream of zeros.

So based on these observations, I wrote a debounce algorithm which considers the
key to be released only when a sufficiently long stream of zeros is observed,
and otherwise considers the key to be pressed. As soon as there is chatter, it
is assumed the key is pressed. The key is then considered to be held as long as
chatter or constant ones continue, and isn't allowed to be considered released
until a stable and decisive stream of zeros is observed.

To prevent spurious presses in case on EMI (not that I noticed any) or a
mechanical disturbance such as dropping the keyboard, I set a relatively low
threshold for the number of pressed observations required before the key is
switched to a pressed state. This should hopefully keep the keys from going
haywire if I even find myself in an environment with a lot of interference.

All of the debouncing logic is encapsulated in the Key class.
