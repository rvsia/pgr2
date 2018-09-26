#ifndef GRAPHICALOBJECT_H
#define GRAPHICALOBJECT_H


class GraphicalObject
{
public:
    GraphicalObject();

    virtual void draw() = 0;

    virtual ~GraphicalObject(){};
};

#endif // GRAPHICALOBJECT_H
