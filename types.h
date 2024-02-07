typedef struct
{
    int X;
    int Y;
} Coordinates;

typedef struct
{
    int ShipSize;
    int Status;
} Field;

typedef struct
{
    int X;
    int Y;
    short Direction; // false - drawing goes down, true - drawing goes right
    short Ok;        // true when finished drawing
} DrawingTable;

typedef struct
{
    short HitSuccessful;
    int X;
    int Y;
} Hit;