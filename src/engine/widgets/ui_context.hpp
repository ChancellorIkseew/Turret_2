#pragma once

class Audio;
class Cursor;
class Input;

struct UIContext {
    Audio& audio;
    Cursor& cursor;
    Input& input;
    UIContext(Audio& audio, Cursor& cursor, Input& input) :
        audio(audio), cursor(cursor), input(input) { }
};
