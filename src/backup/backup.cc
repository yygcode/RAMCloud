#include <backup/backup.h>

#include <shared/backuprpc.h>

#include <cstdio>
#include <cassert>

namespace RAMCloud {

BackupServer::BackupServer(Net *net_impl)
        : net(net_impl)
{
    
}

void
BackupServer::SendRPC(struct backup_rpc *rpc)
{
    net->Send(rpc, rpc->hdr.len);
}

void
BackupServer::RecvRPC(struct backup_rpc **rpc)
{
    size_t len = net->Recv(reinterpret_cast<void**>(rpc));
    assert(len == (*rpc)->hdr.len);
}

void
BackupServer::Heartbeat(const backup_rpc *req, backup_rpc *resp)
{
    resp->hdr.type = BACKUP_RPC_HEARTBEAT_RESP;
    resp->hdr.len = (uint32_t) BACKUP_RPC_HEARTBEAT_RESP_LEN;
    resp->heartbeat_resp.ok = 1;
}

void
BackupServer::Write(const backup_rpc *req, backup_rpc *resp)
{
    resp->hdr.type = BACKUP_RPC_WRITE_RESP;
    resp->hdr.len = (uint32_t) BACKUP_RPC_WRITE_RESP_LEN;
    resp->write_resp.ok = 1;

    printf("Handling Write - total msg len %z\n", req->hdr.len);
    int data_len = req->hdr.len - BACKUP_RPC_HDR_LEN;
    char buf[16384];
    const char *src = req->write_req.data[0];
    strncpy(&buf[0], src, 16384);
    buf[16383] = '\0';
}

void
BackupServer::Commit(const backup_rpc *req, backup_rpc *resp)
{
    resp->hdr.type = BACKUP_RPC_COMMIT_RESP;
    resp->hdr.len = (uint32_t) BACKUP_RPC_COMMIT_RESP_LEN;
    resp->commit_resp.ok = 1;
}

void
BackupServer::HandleRPC()
{
    struct backup_rpc *req;
    struct backup_rpc resp;

    RecvRPC(&req);

    printf("got rpc type: 0x%08x, len 0x%08x\n", req->hdr.type, req->hdr.len);
    
    switch((enum backup_rpc_type) req->hdr.type) {
        case BACKUP_RPC_HEARTBEAT_REQ: Heartbeat(req, &resp); break;
        case BACKUP_RPC_WRITE_REQ:     Write(req, &resp);     break;
        case BACKUP_RPC_COMMIT_REQ:    Commit(req, &resp);    break;
            
        case BACKUP_RPC_HEARTBEAT_RESP:
        case BACKUP_RPC_WRITE_RESP:
        case BACKUP_RPC_COMMIT_RESP:
            throw "server received RPC response";
            
        default:
            throw "received unknown RPC type";
    };
    SendRPC(&resp);
}

void __attribute__ ((noreturn))
BackupServer::Run() {
    while (true)
        HandleRPC();
}


} // namespace RAMCloud

