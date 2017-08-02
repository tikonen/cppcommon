
// bytemask for a bitrange
#define BYTEMASKR(b1, b0) (((0xFF << (b0)) & 0xFF) & (0xFF >> (7 - (b1))))

#define BYTEMASKB(b7, b6, b5, b4, b3, b2, b1, b0) // TODO
