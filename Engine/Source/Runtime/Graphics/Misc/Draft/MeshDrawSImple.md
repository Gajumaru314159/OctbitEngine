- Primitive
  - LOD単位
- Instance
  - オブジェクト単位

```cpp
// Denseではないが単数/複数で効率がいい
template<class T>
class SparseUploadBuffer {
public:
    class Handle {
    public:
        Span<T> view() {
            return { m_cpu.begin() + m_start, m_cpu.begin() + m_start + m_ count };
        }
    private:
        Vector<T>& m_cpu;
        s32 m_start;
        s32 m_count;  
    };
public:
    Handle alloc(s32 size) {
        auto [start,count] = m_allocator.alloc(size);
        // m_cpu.resize(m_allocator.size());
        return Handle{ m_cpu, start, count };
    }
    void free(Handle& handle) {
        m_allocatpr.free(handle.m_start,handle.m_count);
        handle.m_start = 0;
        handle.m_count = 0;
    }
    void reserve() {
        m_cpu.resize(m_allocator.size());
        m_gpu.resize(m_allocator.size());
    }
private:
    Vector<T>     m_cpu;
    Ref<Buffer>   m_gpu;
    SpanAllocator m_allocator;
};
template<class T>
class DenseUploadBuffer {
public:
    class Handle {
    public:
        T& get() {
            return m_cpu.at(m_index);
        }
    private:
        Vector<T>& m_cpu;
        s32 m_index;
    };
public:
    void remove() {

    }
private:
};

class Scene {
public:
    void add(MeshProxy* proxy) {
        auto& unit = m_units[ukey];

        s32 instanceCount =  proxy->instances.size();
        bool bGroup = 1 < instanceCount;
        // 動的インスタンシングが対応されるまで
        bGroup = true;

        if(bGroup) {
            auto& group = unit.groups.emplace_back();
            group.proxy = proxy;
            group.proxy->primitive = m_primitives.alloc(); // 割り当てるのはインデックス
            group.instances = m_instances.alloc(proxy->instances);
        } else {
            // インスタンシングはインデックスで行われるので事前確保する場合はインデックスが安定しないといけない
            auto& single = unit.singles.emplace_back();
            single.proxy = proxy;
            single.proxy->primitive = m_primitives.alloc(); // 割り当てるのはインデックス
            single.instance = m_instances.alloc(proxy->instances); // ※size==1
        }
    }
    void remove(MeshProxy* proxy) {
        auto& unit = m_units[ukey];
        bool bGroup = 1 < instanceCount;

        // Primitive削除
        assert(m_proxies[proxy->primitive]==proxy);
        auto a = proxy->primitive;
        auto b = m_proxies.back()->primitive;
        swap(m_primitives[a],m_primitives[b]);
        m_proxies.back()->primitive = proxy->primitive;

        // Proxy削除
        if(bGroup) {
            m_instances.free(unit.groups[proxy->gindex].instances);
            unit.groups.remove(proxy->gindex);
        } else {
            unit.singles.remove(proxy->gindex);
        }

    }

    void update() {
        for(auto& view: views) {

            // いったんViewごとで書き変わるでOK
            for(auto [i,primitive]: m_primitives.view()) {
                primitive.culled = CalcCulled(view,primitive);
                // カリングされるならLOD判定はいらない
                primitive.level = CalcLevel(view,primitive);
            }

            for(auto& [ukey,unit]: m_units) {

                // シングルのグループを0番とする?
                // インスタンスごとのカリングができないので別にする
                auto& group = unit.dynamicGroup;

                // TODO Viewごとにインスタンス列が変わるので直す必要がある
                for(auto& lod: group.lods) {
                    lod.instances = m_instances.alloc(unit.singles.size());
                }

                // 動的にインスタンシング
                s32 i=0;
                for(auto& single: unit.singles) {
                    auto& primitive = m_primitives[single.proxy->primitive]; 
                    if(primitive.culled) continue;
                    // TODO LODごとのインスタンシング
                    primitive.level;
                    group.instances[i++] = single.instance;
                }
                // group.instances.resize(i);
                group.pritmive.culled = i==0; // すべてculledならPrimitiveごとculled


                // すべてグループとして描画
                for(auto& [bkey,bin]:unit.bins) {
                    bkey.material->shader->passes;
                    for(auto& section: bin.sections) {
                        for(auto& group: unit.groups) {
                            if(m_primitives[group.proxy->primitive].culled) continue;
                            draw(group.instances);
                        }
                    }
                }
                
            }
        }
    }

    // DenseにするならProxy経由で書き換えが必要
    void update() {
        // ProxyはPrimitiveと対応する
        proxy->pindex;
        proxy->sindex;
        m_units[proxy->ukey].singles[proxy->sindex].primitive; // swap
    }
private:
    UploadBuffer<PrimitiveData> m_primitives;
    UploadBuffer<InstanceData>  m_instances;

    Map<MeshUnitKey,MeshUnit> m_units;
};



```