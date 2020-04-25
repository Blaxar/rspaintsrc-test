#include <gst/gst.h>

int
main (int argc, char *argv[])
{
  GstElement *pipeline,
             *rspaintsrc, *compositor, *videoconvert, *videosink,
             *videotestsrc;
  GstBus *bus;
  GstMessage *msg;

  /* Initialize GStreamer */
  gst_init (&argc, &argv);

  /* Build the pipeline */
  pipeline = gst_pipeline_new ("rspaintsrc-test-pipeline");

  rspaintsrc   = gst_element_factory_make ("rspaintsrc", NULL);
  compositor   = gst_element_factory_make ("compositor", NULL);
  videoconvert = gst_element_factory_make ("videoconvert", NULL);
  videosink    = gst_element_factory_make ("autovideosink", NULL);
  videotestsrc = gst_element_factory_make ("videotestsrc", NULL);

  gst_bin_add_many (GST_BIN(pipeline),
                    rspaintsrc, compositor, videoconvert, videosink,
                    videotestsrc, NULL);

  gst_element_link (videotestsrc, compositor);
  gst_element_link_many (rspaintsrc, compositor,
                         videoconvert, videosink,
                         NULL);

  /* Start playing */
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  /* Wait until error or EOS */
  bus = gst_element_get_bus (pipeline);
  msg =
      gst_bus_timed_pop_filtered (bus, GST_CLOCK_TIME_NONE,
      GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Free resources */
  if (msg != NULL)
    gst_message_unref (msg);
  gst_object_unref (bus);
  gst_element_set_state (pipeline, GST_STATE_NULL);
  gst_object_unref (pipeline);
  return 0;
}

