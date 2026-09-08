## 2D Loot & Inventory Game 
A small C++20 & SDL3 loot game built from scratch to explore game programming fundamentals, memory management, and custom 2D rendering.

The project started as a simple console-style grid game using primitive shapes and evolved into a bitmap-based UI with custom icons and a custom-built bitmap font renderer.

## What I Learned
- Performance Profiling : To check the load on the heap memory and CPU
- Pointers & Inventory Systems : Used pointers to manage player inventory and loot objects, gaining a better understanding of object ownership, references, and memory layout.
- RAII & Resource Lifetime : Learned how RAII can tie resource cleanup to object lifetime and make engine components safer to manage.
- Memory Management : Worked directly with allocation, deallocation, object lifetime, and the problems that can occur when ownership is unclear.
- OOP in Game Programming : Structured gameplay around separate Player, Loot, and UI responsibilities instead of putting everything into one system.
- Custom Rendering : Built my own bitmap font renderer and integrated custom BMP icons into the game instead of relying entirely on external UI systems.

## Initial Version
The first version focused on getting the gameplay working.

- Player represented as a simple square
- Loot represented as colored squares
- Grid-based movement and interaction
- Basic inventory functionality
- Minimal UI

This kept the rendering simple so I could focus on the underlying game logic and inventory system.

Initial prototype:

https://github.com/user-attachments/assets/971ff6f8-f107-4c81-aba7-233f872322af

## Latest Version
Once the core gameplay was working, I replaced the primitive shapes with custom assets.

- Custom BMP loot icons
- Custom player/game visuals
- Custom 8×8 bitmap font renderer
- Inventory UI with visual loot representation
- More polished in-game presentation
- Profiled the output for performance analysis
**The new version looked significantly better and made the inventory easier to understand, but it also introduced a performance cost compared with rendering simple rectangles.**

Asset-based version:


https://github.com/user-attachments/assets/645ec6fb-bc40-4892-87ab-c53cf511e658



## Performance Trade-Off

The asset-based renderer currently performs more individual texture operations than the original primitive renderer.

This was a useful engineering trade-off to learn from:

Primitive shapes → lower rendering overhead

Bitmap assets → better visual quality, higher rendering cost

Rather than hiding the trade-off, I kept it as part of the project's progression.

## Future Improvements

The goal is to keep the improved UI and visual quality while bringing the rendering performance closer to the original prototype.


