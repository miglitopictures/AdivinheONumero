// ___math utils_______________________________________________________________________________________

float flerp(float start, float end, float amount){
    return start + (end - start) * amount;
}

float fmap(float value, float fromStart, float fromEnd, float toStart, float toEnd){
    float t = (value - fromStart) / (fromEnd - fromStart);
    return flerp(toStart, toEnd, t);
}

int ilerp(int start, int end, float amount){
    return start + (int) ((end - start) * amount);
}

int imap(int value, int fromStart, int fromEnd, int toStart, int toEnd){
    float t = (float)(value - fromStart) / (float)(fromEnd - fromStart);
    return ilerp(toStart, toEnd, t);
}
