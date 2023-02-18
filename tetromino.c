#include<assert.h>

const char tetrominos[]={
                         0,0,0,0, //I 
                         1,1,1,1,
                         0,0,0,0,
                         0,0,0,0,

                         0,0,1,0,
                         0,0,1,0,
                         0,0,1,0,
                         0,0,1,0,

                         0,0,0,0,
                         1,1,1,1,
                         0,0,0,0,
                         0,0,0,0,

                         0,0,1,0,
                         0,0,1,0,
                         0,0,1,0,
                         0,0,1,0,

                         0,0,0,0, //O
                         0,1,1,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,0,0,
                         0,1,1,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,0,0,
                         0,1,1,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,0,0,
                         0,1,1,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,0,0, //J
                         1,1,1,0,
                         0,0,1,0,
                         0,0,0,0,

                         0,1,0,0,
                         0,1,0,0,
                         1,1,0,0,
                         0,0,0,0,

                         1,0,0,0,
                         1,1,1,0,
                         0,0,0,0,
                         0,0,0,0,

                         0,1,1,0,
                         0,1,0,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,0,0,0, //L
                         1,1,1,0,
                         1,0,0,0,
                         0,0,0,0,

                         1,1,0,0,
                         0,1,0,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,0,1,0,
                         1,1,1,0,
                         0,0,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         0,1,0,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,0,0, //S
                         0,1,1,0,
                         1,1,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         0,1,1,0,
                         0,0,1,0,
                         0,0,0,0,

                         0,0,0,0,
                         0,1,1,0,
                         1,1,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         0,1,1,0,
                         0,0,1,0,
                         0,0,0,0,

                         0,0,0,0, //Z
                         1,1,0,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,1,0,
                         0,1,1,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,0,0,0,
                         1,1,0,0,
                         0,1,1,0,
                         0,0,0,0,

                         0,0,1,0,
                         0,1,1,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,0,0,0, //T
                         1,1,1,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         1,1,0,0,
                         0,1,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         1,1,1,0,
                         0,0,0,0,
                         0,0,0,0,

                         0,1,0,0,
                         0,1,1,0,
                         0,1,0,0,
                         0,0,0,0,
                        };

char get_tetromino_tile(char tetr, int dx, int dy, int rot){
//tetr:
// 0:I 1:O 2:J 3:L 4:L 5:S 6:Z 7:T
        assert(rot >= 0 && rot <= 3 );
        assert(tetr >= 0 && tetr <= 7 );
        assert(dx >= 0 && dx <= 3 );
        assert(dy <= 0 && dy >= -3); //staying safe

        const int index = tetr*64 + rot*16 + dy*4 + dx;
        return tetrominos[index];

}