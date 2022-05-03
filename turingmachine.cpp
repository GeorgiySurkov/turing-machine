#include "turingmachine.h"
#include "helpers.h"

#include <iostream>

#define checkToken(token)                               \
    if (!token) {                                       \
        throw std::invalid_argument("Wrong format");    \
    }

void Program::readFromStream(std::istream &is) {
    Rule currRule;
    const size_t buffSize = 256;
    char buff[buffSize];
    const char *delims = " ", *token;
    bool firstStateNotEncountered = true;
    while (is.getline(buff, buffSize)) {
        token = my_strtok(buff, delims);
        checkToken(token)
        currRule.fromState = parseState(token);
        if (firstStateNotEncountered) {
            _initState = currRule.fromState;
            firstStateNotEncountered = false;
        }

        token = my_strtok(nullptr, delims);
        checkToken(token)
        currRule.fromSymbol = parseSymbol(token);

        token = my_strtok(nullptr, delims);
        checkToken(token)
        if (my_strcmp(token, "->") != 0) {
            throw std::invalid_argument("Invalid arrow sign");
        }

        token = my_strtok(nullptr, delims);
        checkToken(token)
        currRule.toState = parseState(token);

        token = my_strtok(nullptr, delims);
        checkToken(token)
        currRule.toSymbol = parseSymbol(token);

        token = my_strtok(nullptr, delims);
        checkToken(token)
        if (my_strcmp(token, "L") == 0) {
            currRule.move = MOVE_LEFT;
        } else if (my_strcmp(token, "R") == 0) {
            currRule.move = MOVE_RIGHT;
        } else if (my_strcmp(token, "N") == 0) {
            currRule.move = MOVE_STAY;
        } else {
            throw std::invalid_argument("Invalid move sign");
        }

        _rules.push_back(currRule);
    }
}

const Rule &Program::getRule(state_t fromState, alphabet_t fromSymbol) const {
    bool stateExists = false;
    for (Vector<Rule>::iterator it = _rules.begin(); it != _rules.end(); ++it) {
        if (it->fromState == fromState) {
            stateExists = true;
            if (it->fromSymbol == fromSymbol) {
                return *it;
            }
        }
    }
    if (stateExists) {
        throw rule_not_found();
    } else {
        char buff[256];
        snprintf(buff, 256, "No rule for state s%d, symbol %d", fromState, fromSymbol);
        throw std::runtime_error(buff);
    }
}

state_t Program::parseState(const char *s) {
    if (s[0] != 'q') {
        char buff[128];
        snprintf(buff, 128, "Invalid state: %s", s);
        throw std::invalid_argument(buff);
    }
    try {
        return parseInteger<state_t>(++s);
    } catch (const std::invalid_argument &err) {
        char buff[128];
        snprintf(buff, 128, "Invalid state: %s", s);
        throw std::invalid_argument(buff);
    }
}

alphabet_t Program::parseSymbol(const char *s) {
    try {
        return parseInteger<alphabet_t>(s);
    } catch (const std::invalid_argument &err) {
        char buff[128];
        snprintf(buff, 128, "Invalid symbol: %s", s);
        throw std::invalid_argument(buff);
    }
}

std::string TuringMachine::calc(const std::string &init, step_t maxSteps) {
    Vector<int> tape(_tapeSize, 0);
    for (size_t i = 0; i < init.size(); ++i) {
        tape[i] = init[i] - '0';
    }
    size_t pos = 0;
    state_t state = _program.initState();
    step_t currStep;
    for (currStep = 0; currStep < maxSteps; ++currStep) {
        try {
            const Rule &rule = _program.getRule(state, tape[pos]);
            state = rule.toState;
            tape[pos] = rule.toSymbol;
            switch (rule.move) {
                case MOVE_LEFT:
                    if (pos == 0) {
                        pos = tape.size() - 1;
                    } else {
                        --pos;
                    }
                    break;
                case MOVE_RIGHT:
                    pos = (pos + 1) % tape.size();
                    break;
                case MOVE_STAY:
                    break;
            }
        } catch (const rule_not_found &err) {
            break;
        }
    }
    if (currStep == maxSteps) {
        return "Not applicable";
    }

    std::string result;
    size_t i = pos;
    do {
        result.push_back('0' + tape[i]);
        i = (i + 1) % tape.size();
    } while (i != pos);
    size_t newSize = result.size();
    while (result[newSize - 1] == '0') --newSize;
    result.resize(newSize);
    return result;
}
