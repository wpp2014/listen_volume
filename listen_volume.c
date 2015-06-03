/*
title: listen_volume.c
date: 2015-06-02
author: wpp
description: print the volume if the volume changed.
*/

#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <stdio.h>

int elem_callback(snd_mixer_elem_t *ctl, unsigned int mask)
{
	if(mask & SND_CTL_EVENT_MASK_VALUE)
	{
		long volume;
		snd_mixer_selem_get_playback_volume(ctl, SND_MIXER_SCHN_FRONT_LEFT, &volume);

		printf("%ld\n", volume);
	}
}

int mixer_callback(snd_mixer_t *mixer, unsigned int mask, snd_mixer_elem_t *elem)
{
	snd_mixer_elem_set_callback(elem, elem_callback);

	return 0;
}

int main()
{
	long volume;
	snd_mixer_t *mixer;
	snd_mixer_elem_t *master_element;

	snd_mixer_open(&mixer, 0);
	snd_mixer_attach(mixer, "default");
	snd_mixer_selem_register(mixer, NULL, NULL);
	

	snd_mixer_set_callback(mixer, mixer_callback);

	snd_mixer_load(mixer);

	master_element = snd_mixer_first_elem(mixer);
	snd_mixer_selem_set_playback_volume_range(master_element, 0, 100);


	while(1)
		snd_mixer_handle_events(mixer);

	snd_mixer_close(mixer);
	return 0;
}	