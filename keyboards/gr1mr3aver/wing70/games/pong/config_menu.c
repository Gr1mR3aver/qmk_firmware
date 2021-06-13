menu_entry_t paddle_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Thickness",
        .child.menu_handler    = menu_handler_paddle_thickness,
        .child.display_handler = display_handler_paddle_thickness,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Length",
        .child.menu_handler    = menu_handler_paddle_length,
        .child.display_handler = display_handler_paddle_length,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB hue",
        .child.menu_handler    = menu_handler_paddle_rgbhue,
        .child.display_handler = display_handler_paddle_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB saturation",
        .child.menu_handler    = menu_handler_paddle_rgbsat,
        .child.display_handler = display_handler_paddle_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB value",
        .child.menu_handler    = menu_handler_paddle_rgbval,
        .child.display_handler = display_handler_paddle_rgbval,
    }
}

menu_entry_t ball_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Radius",
        .child.menu_handler    = menu_handler_ball_radius,
        .child.display_handler = display_handler_ball_radius,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB hue",
        .child.menu_handler    = menu_handler_ball_rgbhue,
        .child.display_handler = display_handler_ball_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB saturation",
        .child.menu_handler    = menu_handler_ball_rgbsat,
        .child.display_handler = display_handler_ball_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB value",
        .child.menu_handler    = menu_handler_ball_rgbval,
        .child.display_handler = display_handler_ball_rgbval,
    }
}

menu_entry_t court_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Background RGB hue",
        .child.menu_handler    = menu_handler_court_rgbhue,
        .child.display_handler = display_handler_court_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Background RGB saturation",
        .child.menu_handler    = menu_handler_court_rgbsat,
        .child.display_handler = display_handler_court_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Background RGB value",
        .child.menu_handler    = menu_handler_court_rgbval,
        .child.display_handler = display_handler_court_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Line RGB hue",
        .child.menu_handler    = menu_handler_line_rgbhue,
        .child.display_handler = display_handler_line_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Line RGB saturation",
        .child.menu_handler    = menu_handler_line_rgbsat,
        .child.display_handler = display_handler_line_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Line RGB value",
        .child.menu_handler    = menu_handler_line_rgbval,
        .child.display_handler = display_handler_line_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Net uses matrix color",
        .child.menu_handler    = menu_handler_net_color_toggle,
        .child.display_handler = display_handler_net_color_toggle,
    }
}

menu_entry_t line_entries[] = {
}

menu_entry_t net_entries[] = {
}

menu_entry_t game_entries [] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "Score to win",
        .child.menu_handler    = menu_handler_max_score,
        .child.display_handler = display_handler_max_score,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB enabled",
        .child.menu_handler    = menu_handler_rgbenabled,
        .child.display_handler = display_handler_rgbenabled,
    },

}
menu_entry_t pong_entries[] = {
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB enabled",
        .child.menu_handler    = menu_handler_rgbenabled,
        .child.display_handler = display_handler_rgbenabled,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB mode",
        .child.menu_handler    = menu_handler_rgbmode,
        .child.display_handler = display_handler_rgbmode,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB hue",
        .child.menu_handler    = menu_handler_rgbhue,
        .child.display_handler = display_handler_rgbhue,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB saturation",
        .child.menu_handler    = menu_handler_rgbsat,
        .child.display_handler = display_handler_rgbsat,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB value",
        .child.menu_handler    = menu_handler_rgbval,
        .child.display_handler = display_handler_rgbval,
    },
    {
        .flags                 = menu_flag_is_value,
        .text                  = "RGB speed",
        .child.menu_handler    = menu_handler_rgbspeed,
        .child.display_handler = display_handler_rgbspeed,
    },
};
