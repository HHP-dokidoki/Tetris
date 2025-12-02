#ifndef SHAPECONTROL_H

#define SHAPECONTROL_H

int  GetNextShape();

int  NotConflict(int shape_pos_x, int shape_pos_y, int shape_id);

int  Moveable(int shape_pos_x, int shape_pos_y, int shape_id, int mode);

void MoveShape(int* shape_pos_x, int* shape_pos_y, int shape_id, int mode);

void RollShape(int shape_pos_x, int shape_pos_y, int* shape_id);

void FixShape(int shape_pos_x, int shape_pos_y, int shape_id);

#endif