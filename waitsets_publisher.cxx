/*******************************************************************************
 (c) 2005-2015 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/

#include <dds/pub/ddspub.hpp>
#include <rti/util/util.hpp>      // for sleep()
#include <rti/config/Logger.hpp>  // for logging

#include "application.hpp"  // for command line parsing and ctrl-c
#include "waitsets.hpp"

void run_publisher_application(
        unsigned int domain_id,
        unsigned int sample_count)
{
    // Create a DomainParticipant with default QoS.
    dds::domain::DomainParticipant participant(domain_id);

    // Create a Topic -- and automatically register the type.
    dds::topic::Topic<waitsets> topic(participant, "Example waitsets");

    // Retrieve the default DataWriter QoS, from USER_QOS_PROFILES.xml
    dds::pub::qos::DataWriterQos writer_qos =
            dds::core::QosProvider::Default().datawriter_qos();

    // Set policy for reliable writer based on
    // https://community.rti.com/static/documentation/connext-dds/6.1.2/doc/api/connext_dds/api_cpp2/USER_QOS_PROFILES_8xml-example.html
    rti::core::policy::RtpsReliableWriterProtocol writer_policy = rti::core::policy::RtpsReliableWriterProtocol()
          .heartbeat_period(::dds::core::Duration::from_millisecs(100))
          .fast_heartbeat_period(::dds::core::Duration::from_millisecs(10))
          .late_joiner_heartbeat_period(::dds::core::Duration::from_millisecs(10))
          .max_nack_response_delay(::dds::core::Duration::from_millisecs(0))
          .min_nack_response_delay(::dds::core::Duration::from_millisecs(0))
          .heartbeats_per_max_samples(2)
        ;
     writer_qos
        << dds::core::policy::Reliability::Reliable()
        << dds::core::policy::History::KeepLast(10)
        << dds::core::policy::Liveliness::Automatic().lease_duration(dds::core::Duration(2))
        << rti::core::policy::DataWriterProtocol().rtps_reliable_writer(writer_policy)
        ;
     dds::pub::qos::PublisherQos publisher_qos = dds::core::QosProvider::Default().publisher_qos();


     // Create a Publisher
     dds::pub::Publisher publisher(participant);

     // Create a DataWriter.
     dds::pub::DataWriter<waitsets> writer(publisher, topic, writer_qos);

    int64_t samples_written = 0;
    // Main loop. Create a Publisher and DataWriter inside it to reproduce error
    while(!application::shutdown_requested){

        while (!application::shutdown_requested && writer.publication_matched_status().current_count() == 0) {
            // Wait for matched subscription to not send message to nowhere
        }

        samples_written++;
        std::cout << "Writing waitsets, count " << samples_written << std::endl;

        // Create sample and send it.
        waitsets instance;
        instance.x(samples_written);
        writer.write(instance);

        auto start = std::chrono::steady_clock::now();

        // Wait until the reader confirm package receiving
        writer.wait_for_acknowledgments(dds::core::Duration::from_secs(100));

        std::chrono::duration<double, std::milli> time_diff = std::chrono::steady_clock::now() - start;
        std::cout << "ack wait time:" << time_diff.count() << "ms. Subscriptions count: " << writer.publication_matched_status().current_count() << std::endl;
    }
    
    writer.close();
    publisher.close();
}

int main(int argc, char *argv[])
{
    using namespace application;

    // Parse arguments and handle control-C
    auto arguments = parse_arguments(argc, argv);
    if (arguments.parse_result == ParseReturn::exit) {
        return EXIT_SUCCESS;
    } else if (arguments.parse_result == ParseReturn::failure) {
        return EXIT_FAILURE;
    }
    setup_signal_handlers();

    // Sets Connext verbosity to help debugging
    rti::config::Logger::instance().verbosity(arguments.verbosity);

    try {
        run_publisher_application(arguments.domain_id, arguments.sample_count);
    } catch (const std::exception &ex) {
        // This will catch DDS exceptions
        std::cerr << "Exception in run_publisher_application(): " << ex.what()
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Releases the memory used by the participant factory.  Optional at
    // application exit
    dds::domain::DomainParticipant::finalize_participant_factory();

    return EXIT_SUCCESS;
}
