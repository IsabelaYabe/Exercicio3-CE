# Generated by the gRPC Python protocol compiler plugin. DO NOT EDIT!
"""Client and server classes corresponding to protobuf-defined services."""
import grpc
import warnings

import analytics_pb2 as analytics__pb2

GRPC_GENERATED_VERSION = '1.63.0'
GRPC_VERSION = grpc.__version__
EXPECTED_ERROR_RELEASE = '1.65.0'
SCHEDULED_RELEASE_DATE = 'June 25, 2024'
_version_not_supported = False

try:
    from grpc._utilities import first_version_is_lower
    _version_not_supported = first_version_is_lower(GRPC_VERSION, GRPC_GENERATED_VERSION)
except ImportError:
    _version_not_supported = True

if _version_not_supported:
    warnings.warn(
        f'The grpc package installed is at version {GRPC_VERSION},'
        + f' but the generated code in analytics_pb2_grpc.py depends on'
        + f' grpcio>={GRPC_GENERATED_VERSION}.'
        + f' Please upgrade your grpc module to grpcio>={GRPC_GENERATED_VERSION}'
        + f' or downgrade your generated code using grpcio-tools<={GRPC_VERSION}.'
        + f' This warning will become an error in {EXPECTED_ERROR_RELEASE},'
        + f' scheduled for release on {SCHEDULED_RELEASE_DATE}.',
        RuntimeWarning
    )


class AnalyticsServiceStub(object):
    """Missing associated documentation comment in .proto file."""

    def __init__(self, channel):
        """Constructor.

        Args:
            channel: A grpc.Channel.
        """
        self.SendEvent = channel.unary_unary(
                '/analytics.AnalyticsService/SendEvent',
                request_serializer=analytics__pb2.EventRequest.SerializeToString,
                response_deserializer=analytics__pb2.EventResponse.FromString,
                _registered_method=True)
        self.GetLatency = channel.unary_unary(
                '/analytics.AnalyticsService/GetLatency',
                request_serializer=analytics__pb2.LatencyRequest.SerializeToString,
                response_deserializer=analytics__pb2.LatencyResponse.FromString,
                _registered_method=True)


class AnalyticsServiceServicer(object):
    """Missing associated documentation comment in .proto file."""

    def SendEvent(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')

    def GetLatency(self, request, context):
        """Missing associated documentation comment in .proto file."""
        context.set_code(grpc.StatusCode.UNIMPLEMENTED)
        context.set_details('Method not implemented!')
        raise NotImplementedError('Method not implemented!')


def add_AnalyticsServiceServicer_to_server(servicer, server):
    rpc_method_handlers = {
            'SendEvent': grpc.unary_unary_rpc_method_handler(
                    servicer.SendEvent,
                    request_deserializer=analytics__pb2.EventRequest.FromString,
                    response_serializer=analytics__pb2.EventResponse.SerializeToString,
            ),
            'GetLatency': grpc.unary_unary_rpc_method_handler(
                    servicer.GetLatency,
                    request_deserializer=analytics__pb2.LatencyRequest.FromString,
                    response_serializer=analytics__pb2.LatencyResponse.SerializeToString,
            ),
    }
    generic_handler = grpc.method_handlers_generic_handler(
            'analytics.AnalyticsService', rpc_method_handlers)
    server.add_generic_rpc_handlers((generic_handler,))


 # This class is part of an EXPERIMENTAL API.
class AnalyticsService(object):
    """Missing associated documentation comment in .proto file."""

    @staticmethod
    def SendEvent(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/analytics.AnalyticsService/SendEvent',
            analytics__pb2.EventRequest.SerializeToString,
            analytics__pb2.EventResponse.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)

    @staticmethod
    def GetLatency(request,
            target,
            options=(),
            channel_credentials=None,
            call_credentials=None,
            insecure=False,
            compression=None,
            wait_for_ready=None,
            timeout=None,
            metadata=None):
        return grpc.experimental.unary_unary(
            request,
            target,
            '/analytics.AnalyticsService/GetLatency',
            analytics__pb2.LatencyRequest.SerializeToString,
            analytics__pb2.LatencyResponse.FromString,
            options,
            channel_credentials,
            insecure,
            call_credentials,
            compression,
            wait_for_ready,
            timeout,
            metadata,
            _registered_method=True)
