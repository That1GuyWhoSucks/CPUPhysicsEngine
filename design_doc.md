# OPTIMIZE FOR THE COMMON CASE
# DATA LOCALITY IS KEY

# HARDWARE 
i7-1400kf, 80KiB cache size, 64B cache line\
Titan X pascal GPU

# DATA
all the raw data that must be stored, no organization\
size_t = unsigned int\
high_prec = double\
low_prec = float\
ABGR = unsigned int
## Vertex
(x,y,z): high_prec -> position\
(x',y',z'): high_prec -> position'\
(x_vec,y_vec,z_vec): high_prec -> velocity\
m: low_prec -> mass\
o: size_t -> objectID
## Triangle
(p0,p1,p2): size_t -> vertices\
c: ABGR -> color
## Object
*transform: function pointer -> transform\
(w,x,y,z): high_prec -> rotation
AABB: ??? -> aabb
## Camera
(w,x,y,z): high_prec -> cam_rotation\
(x,y,z): high_prec -> cam_position\
c: high_prec -> camera_constant (for math stuff)\
z_buff: high_prec[] -> z_buffer\
buf: ABGR[] -> buffer


# PROCESS
```
INIT
LOOP {
    UPDATE
    PHYSICS
    TRANSFORM
    RENDER
}
DESTROY
```

### INIT
- All

### UPDATE
Unknown user functions are executed in this step\
Common case
- position
- velocity
- rotation
- camera_pos
- canera_rotation
- camera_constant

Uncommon case
- objectID
- mass
- color\
Bad case
- Create new object
  - All
- Delete object
  - All

### PHYSICS
Soft, rigid, and collision are applied in this step
- position
- position'
- velocity
- aabb

### TRANSFORM
Transform simulation space to camera space (before near plane clipping)
- position'
- camera_rotation
- camera_position
- camera_constant

### RENDER
Apply near plane clipping and render triangle if at least 1 vertex is on screen
- position'
- vertices
- color
- z_buffer

## EXPANDABLE ARRAY (PER DATA)
size -> const, size of item\
seg_count -> const, number of segments\
max -> max number of items per segment\
cur -> cur number of items

# LAYOUT
WIP