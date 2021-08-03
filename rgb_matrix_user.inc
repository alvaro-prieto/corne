RGB_MATRIX_EFFECT(custom_effect)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static bool custom_effect(effect_params_t *params) {
	//Since RGB is going to be handled manually, the default effect will be none
    //rgb_matrix_set_color(11, 55, 0, 0);
    //rgb_matrix_set_color_all(20, 0, 0);
    return false;
}

#endif