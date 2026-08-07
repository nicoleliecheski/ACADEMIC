export const SHARDMAP = "shardmap";
export const CLUSTER_EVENTS = "cluster.events";

export const leaseKey = (shardId) => `lease:shard:${shardId}`;
export const docChannel = (docId) => `doc:${docId}`;
export const annotationsChannel = (docId) => `doc:${docId}:annotations`;
export const presenceChannel = (docId) => `doc:${docId}:presence`;
