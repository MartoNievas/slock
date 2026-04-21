/* user and group to drop privileges to */
static const char *user = "nobody";  // Replace this with your username
static const char *group = "nobody"; // this one too

static const char *colorname[NUMCOLS] = {
    [INIT] = "black",     /* after initialization */
    [INPUT] = "#005577",  /* during input */
    [FAILED] = "#CC3333", /* wrong password */
                          /*Does not curently work with BG IMG*/
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 1;

/* Background image path, should be available to the user above */
/*HELLO WORLD*/
static const char *background_image =
    "/home/martin/dev/suckless-btw/slock/slock.png"; // Replace <PATH TO SLOCK>
                                                     // with where you cloned
                                                     // the repo or give path to
                                                     // the image you wanna use
