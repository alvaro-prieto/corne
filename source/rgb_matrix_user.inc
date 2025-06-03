RGB_MATRIX_EFFECT(no_effect)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

/*
When enabling RGB_MATRIX by default, the entire keyboard is repainted
every iteration, even if it's all black. In our implementation, we've
optimized the redraw frequency using a variable that tracks changes or
any drawing necessity. If nothing needs to be drawn, the main loop is
deactivated. We achieve this by creating a do-nothing effect, with
manual drawing management in the "rgb.c" file.
*/

static bool no_effect(effect_params_t *params) {
    return false;
}

#endif
