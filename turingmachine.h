#pragma once

#include <string>
#include <fstream>

#include "vector.h"

typedef int alphabet_t;
typedef int state_t;
typedef int step_t;

class rule_not_found : public std::exception {
    const char * what() const throw() { return "Rule not found"; }
};

enum Move {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_STAY
};

struct Rule {
    state_t fromState;
    alphabet_t fromSymbol;
    state_t toState;
    alphabet_t toSymbol;
    Move move;
};

class Program {
    Vector<Rule> _rules;
    state_t _initState;

public:
    Program() : _initState(-1) {}

    inline state_t initState() const { return _initState; }

    void readFromStream(std::istream &is);

    const Rule &getRule(state_t fromState, alphabet_t fromSymbol) const;

private:
    static state_t parseState(const char *s);

    static alphabet_t parseSymbol(const char *s);

    template<typename Integer>
    static Integer parseInteger(const char *s);
};

template<typename Integer>
Integer Program::parseInteger(const char *s) {
    Integer res = 0;
    for (const char *curr = s; *curr != 0; ++curr) {
        if ('0' <= *curr && *curr <= '9') {
            res = (res * 10) + (*curr - '0');
        } else {
            char err[128];
            snprintf(err, 128, "Char \"%c\" is not decimal digit", *curr);
            throw std::invalid_argument(err);
        }
    }
    return res;
}

class TuringMachine {
    size_t _tapeSize;
    Program _program;

public:
    TuringMachine(size_t tapeSize, const Program &program) :
            _tapeSize(tapeSize),
            _program(program) {}

    std::string calc(std::string const &init, step_t maxSteps);
};
